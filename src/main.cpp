#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <DHT.h>

#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASS "YOUR_PASS"

#define DHT_PIN_1 4
#define DHT_PIN_2 14
#define DHT_TYPE DHT22

DHT dht1(DHT_PIN_1, DHT_TYPE);
DHT dht2(DHT_PIN_2, DHT_TYPE);

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // SPIFFS FILESYSTEM
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed!");
    return;
  }

  // WIFI
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  // DHT SENSORS
  dht1.begin();
  dht2.begin();

  // API JSON
  server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request){
    float t1 = dht1.readTemperature();
    float h1 = dht1.readHumidity();
    float t2 = dht2.readTemperature();
    float h2 = dht2.readHumidity();

    String json = "{";
    json += "\"salle1\":{\"temp\":"+String(t1,1)+",\"hum\":"+String(h1,0)+",\"status\":\"Normal\"},";
    json += "\"salle2\":{\"temp\":"+String(t2,1)+",\"hum\":"+String(h2,0)+",\"status\":\"Normal\"}";
    json += "}";

    request->send(200, "application/json", json);
  });

  // Serve the JS web app
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.begin();
}

void loop() {}