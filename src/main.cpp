/* ================================================================
 * ESP32 – Monitoring DHT22 ×2 | Adafruit IO + Thinger.io + LCD
 * ================================================================
 *
 * BIBLIOTHÈQUES REQUISES (Gestionnaire de bibliothèques Arduino) :
 *   1. "DHT sensor library"       by Adafruit
 *   2. "Adafruit Unified Sensor"  by Adafruit   (dépendance DHT)
 *   3. "Adafruit IO Arduino"      by Adafruit   (publie vers Adafruit IO)
 *   4. "LiquidCrystal I2C"        by Frank de Brabander
 *   5. "Thinger.io"               by thinger.io (fournit ThingerESP32.h)
 *   6. ESPAsyncWebServer + AsyncTCP (dashboard local) :
 *        https://github.com/me-no-dev/ESPAsyncWebServer
 *        https://github.com/me-no-dev/AsyncTCP
 *
 * PlatformIO : dépendances déclarées dans platformio.ini
 *
 * ================================================================
 * MATÉRIEL
 * ================================================================
 *   DHT22 #1  → GPIO  4      DHT22 #2   → GPIO 14
 *   LCD I2C   → SDA=21, SCL=22  (adresse 0x27 ou 0x3F)
 *   Relais    → GPIO 26      Buzzer     → GPIO 25
 *
 * ================================================================
 * CONFIGURATION — renseignez vos identifiants ci-dessous
 * ================================================================ */

// ── WiFi ─────────────────────────────────────────────────────────
const char* WIFI_SSID     = "VOTRE_SSID";
const char* WIFI_PASSWORD = "VOTRE_MDP_WIFI";

// ── Adafruit IO ──────────────────────────────────────────────────
// Créez un compte sur https://io.adafruit.com puis copiez vos clés
#define AIO_USERNAME  "VOTRE_AIO_USERNAME"
#define AIO_KEY       "VOTRE_AIO_KEY"

// ── Thinger.io ───────────────────────────────────────────────────
// Créez un compte sur https://thinger.io et un "device" ESP32
#define THINGER_USERNAME "VOTRE_THINGER_USERNAME"
#define THINGER_DEVICE   "VOTRE_THINGER_DEVICE_ID"
#define THINGER_TOKEN    "VOTRE_THINGER_DEVICE_TOKEN"

/* ================================================================
 * INCLUDES
 * ================================================================ */
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Adafruit IO – gère WiFi + MQTT vers io.adafruit.com
#include "AdafruitIO_WiFi.h"

// Thinger.io – utilise le WiFi déjà établi par Adafruit IO
//   Note : on n'appelle PAS thing.add_wifi() afin de ne pas
//          interférer avec la connexion gérée par AdafruitIO_WiFi.
#include <ThingerESP32.h>

// Dashboard local via SPIFFS
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>

/* ================================================================
 * PINS & CONSTANTES
 * ================================================================ */
#define DHT_PIN_1   4
#define DHT_PIN_2   14
#define DHT_TYPE    DHT22

#define RELAY_PIN   26
#define BUZZER_PIN  25

#define LCD_ADDR    0x27  // Essayez 0x3F si l'écran reste noir
#define LCD_COLS    16
#define LCD_ROWS    2

// Seuils d'alerte
#define TEMP_MAX    30.0f   // °C
#define TEMP_MIN    15.0f   // °C
#define HUM_MAX     60.0f   // %

// Intervalle de publication vers les clouds (ms)
#define PUBLISH_MS  10000UL

/* ================================================================
 * OBJETS
 * ================================================================ */
DHT dht1(DHT_PIN_1, DHT_TYPE);
DHT dht2(DHT_PIN_2, DHT_TYPE);

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// Adafruit IO
AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASSWORD);

// Feeds Adafruit IO
AdafruitIO_Feed* feed_s1_temp = io.feed("salle1-temp");
AdafruitIO_Feed* feed_s1_hum  = io.feed("salle1-humidity");
AdafruitIO_Feed* feed_s2_temp = io.feed("salle2-temp");
AdafruitIO_Feed* feed_s2_hum  = io.feed("salle2-humidity");
AdafruitIO_Feed* feed_ac      = io.feed("ac-status");
AdafruitIO_Feed* feed_system  = io.feed("system-status");

// Thinger.io
ThingerESP32 thing(THINGER_USERNAME, THINGER_DEVICE, THINGER_TOKEN);

// Dashboard local
AsyncWebServer webServer(80);

/* ================================================================
 * VARIABLES GLOBALES
 * ================================================================ */
float t1 = 0.0f, h1 = 0.0f;  // Salle 1
float t2 = 0.0f, h2 = 0.0f;  // Salle 2
bool  alertActive = false;
bool  acON        = false;
unsigned long lastPublish = 0;

/* ================================================================
 * UTILITAIRES
 * ================================================================ */
bool checkAlerts() {
  return (t1 > TEMP_MAX || t1 < TEMP_MIN ||
          t2 > TEMP_MAX || t2 < TEMP_MIN ||
          h1 > HUM_MAX  || h2 > HUM_MAX);
}

String buildJsonResponse() {
  String st1 = (t1 > TEMP_MAX || t1 < TEMP_MIN || h1 > HUM_MAX) ? "ALERTE" : "Normal";
  String st2 = (t2 > TEMP_MAX || t2 < TEMP_MIN || h2 > HUM_MAX) ? "ALERTE" : "Normal";
  String json = "{";
  json += "\"salle1\":{\"temp\":" + String(t1, 1) + ",\"hum\":" + String(h1, 0) + ",\"status\":\"" + st1 + "\"},";
  json += "\"salle2\":{\"temp\":" + String(t2, 1) + ",\"hum\":" + String(h2, 0) + ",\"status\":\"" + st2 + "\"},";
  json += "\"ac\":"    + String(acON        ? "true" : "false") + ",";
  json += "\"alert\":" + String(alertActive ? "true" : "false");
  json += "}";
  return json;
}

/* ================================================================
 * SETUP
 * ================================================================ */
void setup() {
  Serial.begin(115200);

  // ── GPIO ────────────────────────────────────────────────────
  pinMode(RELAY_PIN,  OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RELAY_PIN,  LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // ── LCD ─────────────────────────────────────────────────────
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("ESP32 DHT22");
  lcd.setCursor(0, 1); lcd.print("Démarrage...");

  // ── Capteurs DHT ────────────────────────────────────────────
  dht1.begin();
  dht2.begin();

  // ── SPIFFS (dashboard local) ─────────────────────────────────
  if (!SPIFFS.begin(true)) {
    Serial.println("[SPIFFS] Echec du montage !");
  }

  // ── Connexion Adafruit IO (gère également le WiFi) ───────────
  Serial.print("[AIO] Connexion");
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Connexion WiFi..");
  io.connect();
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n[AIO] Connecte ! IP : " + WiFi.localIP().toString());
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("WiFi + AIO OK");
  lcd.setCursor(0, 1); lcd.print(WiFi.localIP());
  delay(1500);

  // ── Ressources Thinger.io ────────────────────────────────────
  // Lecture Salle 1
  thing["salle1"] >> [](pson& out) {
    out["temperature"] = t1;
    out["humidity"]    = h1;
    out["alert"]       = (t1 > TEMP_MAX || t1 < TEMP_MIN || h1 > HUM_MAX);
  };

  // Lecture Salle 2
  thing["salle2"] >> [](pson& out) {
    out["temperature"] = t2;
    out["humidity"]    = h2;
    out["alert"]       = (t2 > TEMP_MAX || t2 < TEMP_MIN || h2 > HUM_MAX);
  };

  // Commande relais / climatisation
  thing["ac-control"] << [](pson& in) {
    if (in.is_empty()) { in = acON; return; }  // lecture de l'état actuel
    acON = (bool)in;
    digitalWrite(RELAY_PIN, acON ? HIGH : LOW);
  };

  // ── Dashboard local (ESPAsyncWebServer) ──────────────────────
  // Endpoint JSON
  webServer.on("/api/data", HTTP_GET, [](AsyncWebServerRequest* req) {
    req->send(200, "application/json", buildJsonResponse());
  });

  // Endpoint contrôle relais via POST /api/relay?state=1
  webServer.on("/api/relay", HTTP_POST, [](AsyncWebServerRequest* req) {
    if (req->hasParam("state", true)) {
      String v = req->getParam("state", true)->value();
      acON = (v == "1" || v == "on" || v == "true");
      digitalWrite(RELAY_PIN, acON ? HIGH : LOW);
    }
    req->send(200, "application/json", "{\"ok\":true}");
  });

  // Servir l'application web depuis SPIFFS
  webServer.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  webServer.begin();

  Serial.println("[WEB] Dashboard local : http://" + WiFi.localIP().toString());
}

/* ================================================================
 * LOOP
 * ================================================================ */
void loop() {
  io.run();       // Keepalive Adafruit IO MQTT
  thing.handle(); // Keepalive Thinger.io

  unsigned long now = millis();
  if (now - lastPublish < PUBLISH_MS) return;
  lastPublish = now;

  // ── Lecture capteurs ──────────────────────────────────────
  float nT1 = dht1.readTemperature();
  float nH1 = dht1.readHumidity();
  float nT2 = dht2.readTemperature();
  float nH2 = dht2.readHumidity();

  if (!isnan(nT1)) t1 = nT1;
  if (!isnan(nH1)) h1 = nH1;
  if (!isnan(nT2)) t2 = nT2;
  if (!isnan(nH2)) h2 = nH2;

  alertActive = checkAlerts();

  // ── Alertes : buzzer + activation automatique du relais ───
  // Note : en cas d'alerte, le relais est forcé ON automatiquement.
  // Un contrôle manuel (dashboard / Thinger.io) ne peut désactiver
  // le relais que lorsque toutes les conditions d'alerte sont levées.
  if (alertActive) {
    digitalWrite(BUZZER_PIN, HIGH);
    if (!acON) {
      acON = true;
      digitalWrite(RELAY_PIN, HIGH);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // ── Affichage LCD ─────────────────────────────────────────
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1:"); lcd.print(t1, 1); lcd.print((char)223); lcd.print("C ");
  lcd.print(h1, 0); lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("S2:"); lcd.print(t2, 1); lcd.print((char)223); lcd.print("C ");
  lcd.print(h2, 0); lcd.print("%");

  if (alertActive) {
    delay(800);
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("!! ALERTE !!");
    lcd.setCursor(0, 1); lcd.print(acON ? "AC: ON " : "AC: OFF");
  }

  // ── Publication Adafruit IO ───────────────────────────────
  feed_s1_temp->save(t1);
  feed_s1_hum ->save(h1);
  feed_s2_temp->save(t2);
  feed_s2_hum ->save(h2);
  feed_ac     ->save(acON        ? "ON"     : "OFF");
  feed_system ->save(alertActive ? "ALERTE" : "Normal");

  // ── Debug série ───────────────────────────────────────────
  Serial.printf("[S1] T=%.1f H=%.0f  [S2] T=%.1f H=%.0f  Alert=%s  AC=%s\n",
                t1, h1, t2, h2,
                alertActive ? "OUI" : "NON",
                acON        ? "ON"  : "OFF");
}