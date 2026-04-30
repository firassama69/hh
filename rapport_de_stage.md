# Rapport de Stage

---

## Page de Garde

| | |
|---|---|
| **Intitulé du projet** | Système de surveillance de température et d'humidité connecté basé sur ESP32 |
| **Stagiaire** | [Votre Nom Prénom] |
| **Établissement** | [Nom de votre école / université] |
| **Filière** | [Votre filière, ex. : Génie Électronique / Informatique Industrielle] |
| **Niveau** | [ex. : 2ème année BTS / Licence 3 / Master 1] |
| **Entreprise d'accueil** | [Nom de l'entreprise] |
| **Tuteur de stage** | [Nom du tuteur] |
| **Durée du stage** | [Date début] – [Date fin] |
| **Année universitaire** | 2025 – 2026 |

---

## Remerciements

Je tiens à exprimer ma sincère gratitude envers toutes les personnes qui ont contribué au bon déroulement de ce stage et à la réalisation de ce projet.

Je remercie tout particulièrement mon tuteur de stage, **[Nom du tuteur]**, pour ses précieux conseils, sa disponibilité et son encadrement tout au long de cette période.

Je remercie également l'ensemble de l'équipe de **[Nom de l'entreprise]** pour leur accueil chaleureux et le partage de leurs connaissances et expériences.

Enfin, je remercie mes enseignants de **[Nom de l'établissement]** pour la formation solide qu'ils m'ont dispensée, qui m'a permis de mener à bien ce projet.

---

## Sommaire

1. [Introduction](#1-introduction)
2. [Présentation de l'entreprise](#2-présentation-de-lentreprise)
3. [Contexte et problématique du projet](#3-contexte-et-problématique-du-projet)
4. [Analyse des besoins et cahier des charges](#4-analyse-des-besoins-et-cahier-des-charges)
5. [Architecture et conception du système](#5-architecture-et-conception-du-système)
6. [Réalisation technique](#6-réalisation-technique)
   - 6.1 [Matériel utilisé](#61-matériel-utilisé)
   - 6.2 [Environnement de développement](#62-environnement-de-développement)
   - 6.3 [Firmware ESP32](#63-firmware-esp32)
   - 6.4 [Interface web (Dashboard)](#64-interface-web-dashboard)
7. [Tests et validation](#7-tests-et-validation)
8. [Difficultés rencontrées et solutions apportées](#8-difficultés-rencontrées-et-solutions-apportées)
9. [Bilan et acquis du stage](#9-bilan-et-acquis-du-stage)
10. [Conclusion](#10-conclusion)
11. [Annexes](#11-annexes)

---

## 1. Introduction

Dans un monde de plus en plus connecté, l'Internet des Objets (IoT) occupe une place centrale dans la transformation numérique des entreprises et des infrastructures. La surveillance en temps réel des conditions environnementales — notamment la température et l'humidité — représente un enjeu crucial dans de nombreux secteurs : industrie, agriculture, santé, bâtiment intelligent, et bien d'autres.

C'est dans ce contexte que **[Nom de l'entreprise]** m'a confié, dans le cadre de mon stage de [durée] au sein de son équipe technique, la conception et le développement d'un **système de surveillance connecté** basé sur le microcontrôleur **ESP32**.

Le présent rapport a pour objectif de décrire les différentes étapes de ce projet, depuis l'analyse des besoins jusqu'à la mise en œuvre du système, en passant par les choix techniques et les difficultés rencontrées.

Ce document est structuré de la manière suivante : après une présentation de l'entreprise d'accueil, nous exposons le contexte et la problématique du projet, puis nous détaillons l'architecture, la réalisation technique, les tests effectués, et enfin nous dressons un bilan de cette expérience.

---

## 2. Présentation de l'entreprise

### 2.1 Présentation générale

**[Nom de l'entreprise]** est une entreprise spécialisée dans **[domaine d'activité, ex. : l'automatisation industrielle / les systèmes embarqués / l'ingénierie électronique]**, fondée en **[année]** et dont le siège social est situé à **[ville, pays]**.

Elle intervient principalement dans les secteurs de **[ex. : l'industrie, le bâtiment, l'agriculture connectée]** et propose des solutions techniques sur mesure à ses clients.

### 2.2 Chiffres clés

| Indicateur | Valeur |
|---|---|
| Effectif | [Nombre] employés |
| Chiffre d'affaires | [Montant] |
| Année de création | [Année] |
| Secteur | [Secteur d'activité] |

### 2.3 Organisation et service d'accueil

J'ai intégré le département **[Nom du département, ex. : R&D / Bureau d'études / IoT & Systèmes embarqués]**, composé de **[nombre]** ingénieurs et techniciens. Ce service est chargé de **[description des missions du service]**.

---

## 3. Contexte et problématique du projet

### 3.1 Contexte

L'entreprise dispose de plusieurs **salles techniques** (salles serveurs, salles de production, entrepôts, etc.) dont les conditions thermiques et hygrométriques doivent être surveillées en permanence afin de :

- Prévenir les risques de surchauffe des équipements ;
- Garantir des conditions de stockage ou de fabrication conformes aux normes ;
- Détecter rapidement toute anomalie environnementale.

Jusqu'à présent, le suivi de ces paramètres était effectué manuellement, avec des relevés périodiques peu fiables et sans historique structuré.

### 3.2 Problématique

> **Comment mettre en place un système IoT simple, peu coûteux et accessible, permettant la surveillance en temps réel de la température et de l'humidité dans plusieurs salles, avec une interface de visualisation accessible depuis n'importe quel appareil connecté au réseau local ?**

### 3.3 Objectifs du projet

- Acquérir les données de température et d'humidité de deux salles via des capteurs **DHT22** ;
- Traiter et exposer ces données sous forme d'une **API REST JSON** ;
- Développer un **tableau de bord web** (dashboard) accessible depuis un navigateur ;
- Déployer le tout sur un microcontrôleur **ESP32** connecté au réseau Wi-Fi de l'entreprise.

---

## 4. Analyse des besoins et cahier des charges

### 4.1 Besoins fonctionnels

| ID | Besoin | Priorité |
|---|---|---|
| BF01 | Lire la température et l'humidité de la Salle 1 | Haute |
| BF02 | Lire la température et l'humidité de la Salle 2 | Haute |
| BF03 | Exposer les données via une API REST (JSON) | Haute |
| BF04 | Afficher un dashboard web accessible via navigateur | Haute |
| BF05 | Rafraîchissement automatique des données toutes les 2 secondes | Moyenne |
| BF06 | Affichage du statut de chaque salle (Normal / Alerte) | Moyenne |

### 4.2 Besoins non fonctionnels

| ID | Besoin | Priorité |
|---|---|---|
| BNF01 | Faible coût de la solution matérielle | Haute |
| BNF02 | Autonomie et embarquabilité (système standalone) | Haute |
| BNF03 | Accessibilité depuis n'importe quel appareil sur le réseau local | Haute |
| BNF04 | Interface responsive et lisible | Moyenne |
| BNF05 | Code modulaire et maintenable | Moyenne |

### 4.3 Contraintes techniques

- Utilisation du framework **Arduino** via **PlatformIO** ;
- Connexion au réseau Wi-Fi de l'entreprise ;
- Stockage des fichiers web sur le système de fichiers **SPIFFS** de l'ESP32 ;
- Microcontrôleur : **ESP32 DevKit**.

---

## 5. Architecture et conception du système

### 5.1 Architecture globale

Le système est composé de trois couches :

```
┌─────────────────────────────────────────────┐
│           COUCHE PRÉSENTATION               │
│   Interface Web (HTML/CSS/JS) – Dashboard   │
│         Navigateur (PC, mobile)             │
└───────────────────┬─────────────────────────┘
                    │ HTTP / REST API
┌───────────────────▼─────────────────────────┐
│           COUCHE APPLICATIVE                │
│   ESP32 – Serveur Web Asynchrone (port 80)  │
│   Endpoint : GET /api/data → JSON           │
│   Fichiers statiques servis via SPIFFS      │
└───────────────────┬─────────────────────────┘
                    │ GPIO (lecture analogique/digitale)
┌───────────────────▼─────────────────────────┐
│           COUCHE ACQUISITION                │
│   Capteur DHT22 #1 (GPIO 4) – Salle 1       │
│   Capteur DHT22 #2 (GPIO 14) – Salle 2      │
└─────────────────────────────────────────────┘
```

### 5.2 Schéma de connexion matérielle

| Composant | Broche ESP32 |
|---|---|
| DHT22 #1 – Data | GPIO 4 |
| DHT22 #2 – Data | GPIO 14 |
| DHT22 #1 & #2 – VCC | 3.3V |
| DHT22 #1 & #2 – GND | GND |

> Une résistance de tirage (pull-up) de **10 kΩ** est recommandée entre la broche DATA et VCC de chaque capteur DHT22.

### 5.3 Format des données API

L'API REST expose un endpoint unique : `GET /api/data`

**Exemple de réponse JSON :**
```json
{
  "salle1": {
    "temp": 23.5,
    "hum": 58,
    "status": "Normal"
  },
  "salle2": {
    "temp": 26.1,
    "hum": 62,
    "status": "Normal"
  }
}
```

---

## 6. Réalisation technique

### 6.1 Matériel utilisé

| Composant | Description | Quantité |
|---|---|---|
| ESP32 DevKit v1 | Microcontrôleur Wi-Fi/Bluetooth | 1 |
| Capteur DHT22 | Capteur de température et humidité | 2 |
| Résistance 10 kΩ | Résistance pull-up pour DHT22 | 2 |
| Câbles Dupont | Connexions prototype | — |
| Breadboard | Carte de prototypage | 1 |

### 6.2 Environnement de développement

| Outil | Version / Description |
|---|---|
| **PlatformIO** | IDE et gestionnaire de build (extension VS Code) |
| **Framework Arduino** | Framework de programmation pour ESP32 |
| **ESPAsyncWebServer** | Serveur web asynchrone pour ESP32 |
| **AsyncTCP** | Couche TCP asynchrone (dépendance) |
| **DHT sensor library (Adafruit)** | Lecture des capteurs DHT11/DHT22 |
| **ArduinoJson** | Sérialisation/désérialisation JSON |
| **SPIFFS** | Système de fichiers flash de l'ESP32 |

**Fichier de configuration `platformio.ini` :**
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

board_build.filesystem = spiffs

lib_deps =
  me-no-dev/ESPAsyncWebServer @ ^1.2.3
  me-no-dev/AsyncTCP @ ^1.1.1
  adafruit/DHT sensor library
  bblanchon/ArduinoJson
```

### 6.3 Firmware ESP32

Le firmware est développé en **C++ (Arduino)** et réalise les opérations suivantes :

#### a) Initialisation

```cpp
void setup() {
  // 1. Montage du système de fichiers SPIFFS
  SPIFFS.begin(true);

  // 2. Connexion au réseau Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); }

  // 3. Initialisation des capteurs DHT22
  dht1.begin();  // GPIO 4
  dht2.begin();  // GPIO 14

  // 4. Démarrage du serveur web
  server.begin();
}
```

#### b) API REST – Endpoint `/api/data`

À chaque requête HTTP GET sur `/api/data`, l'ESP32 :
1. Lit la température et l'humidité des deux capteurs DHT22 ;
2. Construit une réponse JSON ;
3. Renvoie la réponse avec le type MIME `application/json`.

```cpp
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
```

#### c) Serveur de fichiers statiques

Les fichiers HTML/CSS/JS sont stockés dans la mémoire flash SPIFFS et servis directement :

```cpp
server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
```

### 6.4 Interface web (Dashboard)

Le tableau de bord est une **Single Page Application (SPA)** légère développée en **HTML5 / CSS3 / JavaScript vanilla**, stockée dans le fichier `data/index.html` et téléversée sur le SPIFFS de l'ESP32.

#### Fonctionnalités :
- Affichage des données en **temps réel** (rafraîchissement toutes les 2 secondes) ;
- Présentation sous forme de **cartes (cards)** pour chaque salle ;
- Affichage de la **température** (°C), de l'**humidité** (%) et du **statut** ;
- Gestion des erreurs de connexion avec message utilisateur.

#### Aperçu de l'interface :

```
┌─────────────────────────────────────┐
│      ESP32 – DHT22 Dashboard        │
├─────────────────────────────────────┤
│  ┌─────────────────────────────┐    │
│  │  Salle 1                    │    │
│  │  Température : 23.5 °C      │    │
│  │  Humidité : 58 %            │    │
│  │  Statut : Normal            │    │
│  └─────────────────────────────┘    │
│  ┌─────────────────────────────┐    │
│  │  Salle 2                    │    │
│  │  Température : 26.1 °C      │    │
│  │  Humidité : 62 %            │    │
│  │  Statut : Normal            │    │
│  └─────────────────────────────┘    │
└─────────────────────────────────────┘
```

#### Logique JavaScript :

```javascript
async function update() {
  let res = await fetch("/api/data");
  let data = await res.json();
  // Mise à jour du DOM avec les valeurs reçues
}
setInterval(update, 2000);  // Rafraîchissement toutes les 2 secondes
```

---

## 7. Tests et validation

### 7.1 Tests unitaires du firmware

| Test | Méthode | Résultat |
|---|---|---|
| Lecture capteur DHT22 #1 (Salle 1) | Moniteur série (115200 baud) | ✅ Valeurs cohérentes |
| Lecture capteur DHT22 #2 (Salle 2) | Moniteur série | ✅ Valeurs cohérentes |
| Connexion Wi-Fi | Vérification adresse IP affichée | ✅ Connexion établie |
| Montage SPIFFS | Log série | ✅ Système de fichiers monté |

### 7.2 Tests de l'API REST

| Requête | URL | Réponse attendue | Résultat |
|---|---|---|---|
| GET /api/data | `http://<IP_ESP32>/api/data` | JSON avec salle1 & salle2 | ✅ Conforme |
| GET / | `http://<IP_ESP32>/` | Page HTML (index.html) | ✅ Conforme |

### 7.3 Tests de l'interface web

| Scénario | Appareil testé | Résultat |
|---|---|---|
| Accès au dashboard | PC (navigateur Chrome) | ✅ Fonctionne |
| Accès au dashboard | Smartphone (Wi-Fi) | ✅ Fonctionne |
| Rafraîchissement automatique | PC | ✅ Données mises à jour toutes les 2s |
| Déconnexion ESP32 | PC | ✅ Message d'erreur affiché |

---

## 8. Difficultés rencontrées et solutions apportées

### 8.1 Problèmes de lecture des capteurs DHT22

**Problème :** Les capteurs DHT22 renvoyaient parfois des valeurs `NaN` (Not a Number).

**Cause :** Le DHT22 nécessite un délai minimum de 2 secondes entre deux lectures. De plus, l'absence de résistance pull-up peut provoquer des lectures instables.

**Solution :** Ajout de résistances pull-up de 10 kΩ sur les broches DATA, et gestion des valeurs `NaN` dans le code (retour de la dernière valeur valide ou affichage d'un message d'erreur).

---

### 8.2 Taille mémoire du SPIFFS

**Problème :** L'upload du fichier `index.html` échouait avec un message de taille insuffisante.

**Cause :** La partition SPIFFS n'était pas correctement dimensionnée dans la configuration.

**Solution :** Ajout de la directive `board_build.filesystem = spiffs` dans `platformio.ini` et utilisation de la commande PlatformIO *"Upload Filesystem Image"* pour téléverser les fichiers correctement.

---

### 8.3 Blocage de la boucle principale

**Problème :** L'utilisation initiale d'un serveur web synchrone bloquait le programme principal.

**Solution :** Adoption de la bibliothèque **ESPAsyncWebServer** qui gère les requêtes HTTP de manière asynchrone, sans bloquer la boucle `loop()`, grâce à la bibliothèque **AsyncTCP**.

---

### 8.4 Sécurisation des identifiants Wi-Fi

**Problème :** Les identifiants Wi-Fi (`WIFI_SSID` / `WIFI_PASS`) étaient définis en dur dans le code source, ce qui pose un problème de sécurité.

**Piste d'amélioration :** Utilisation d'un portail captif de configuration (mode AP) au premier démarrage, ou stockage des identifiants dans la mémoire non volatile (NVS) de l'ESP32.

---

## 9. Bilan et acquis du stage

### 9.1 Compétences techniques acquises

Ce stage m'a permis de développer et de renforcer les compétences suivantes :

| Domaine | Compétence acquise |
|---|---|
| **Systèmes embarqués** | Programmation ESP32 avec le framework Arduino via PlatformIO |
| **Capteurs IoT** | Interfaçage et lecture de capteurs DHT22 (température/humidité) |
| **Protocoles réseau** | Mise en place d'un serveur HTTP asynchrone, conception d'une API REST |
| **Développement web** | Création d'une interface HTML/JS de visualisation en temps réel |
| **Systèmes de fichiers** | Utilisation du SPIFFS pour le stockage de fichiers sur microcontrôleur |
| **Gestion de projet** | Analyse des besoins, conception, développement, tests |

### 9.2 Compétences transversales

- **Autonomie** : J'ai été amené à rechercher et analyser la documentation technique des composants et bibliothèques utilisés ;
- **Résolution de problèmes** : Chaque difficulté technique m'a conduit à diagnostiquer, rechercher des solutions et les mettre en œuvre ;
- **Communication** : Présentation régulière de l'avancement du projet à mon tuteur.

### 9.3 Apports personnels

Cette expérience en entreprise m'a conforté dans mon intérêt pour les **systèmes embarqués et l'IoT**. Elle m'a permis de confronter les connaissances théoriques acquises en cours à la réalité d'un projet industriel, avec ses contraintes de coût, de délai et de fiabilité.

---

## 10. Conclusion

Ce stage m'a permis de concevoir et de réaliser de bout en bout un **système IoT de surveillance environnementale** basé sur l'ESP32. Le projet répond pleinement aux objectifs fixés : acquisition des données de température et d'humidité en temps réel, exposition via une API REST, et visualisation sur un tableau de bord web accessible depuis le réseau local.

Les principales fonctionnalités développées sont :
- Lecture de deux capteurs DHT22 (Salle 1 et Salle 2) ;
- Serveur web asynchrone embarqué sur l'ESP32 ;
- Interface utilisateur web légère et responsive ;
- Rafraîchissement automatique des données toutes les 2 secondes.

**Perspectives d'évolution :**
- Ajout d'un système d'alertes (e-mail, SMS) en cas de dépassement de seuils ;
- Intégration d'une base de données pour l'historisation des mesures ;
- Ajout d'un portail captif pour la configuration Wi-Fi ;
- Extension à davantage de salles et de capteurs ;
- Sécurisation de l'API (authentification HTTP).

Ce stage a été une expérience enrichissante, tant sur le plan technique qu'humain, et m'a fourni des bases solides pour ma future carrière dans le domaine des systèmes embarqués et de l'IoT.

---

## 11. Annexes

### Annexe A – Code source complet du firmware (`src/main.cpp`)

```cpp
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
```

### Annexe B – Interface Web (`data/index.html`)

```html
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Dashboard ESP32</title>
<style>
  body { background: #f2f2f2; font-family: Arial; padding: 20px; }
  .card { background: white; padding: 20px; border-radius: 10px;
          margin-bottom: 20px; box-shadow: 0px 2px 6px rgba(0,0,0,0.2); }
  .title { font-size: 22px; font-weight: bold; margin-bottom: 10px; }
  .value { font-size: 18px; margin-bottom: 5px; }
</style>
</head>
<body>
<h1>ESP32 – DHT22 Dashboard</h1>
<div id="salle1" class="card"></div>
<div id="salle2" class="card"></div>
<script>
async function update() {
  try {
    let res = await fetch("/api/data");
    let data = await res.json();
    document.getElementById("salle1").innerHTML = `
      <div class="title">Salle 1</div>
      <div class="value">Température: ${data.salle1.temp} °C</div>
      <div class="value">Humidité: ${data.salle1.hum} %</div>
      <div class="value">Statut: ${data.salle1.status}</div>`;
    document.getElementById("salle2").innerHTML = `
      <div class="title">Salle 2</div>
      <div class="value">Température: ${data.salle2.temp} °C</div>
      <div class="value">Humidité: ${data.salle2.hum} %</div>
      <div class="value">Statut: ${data.salle2.status}</div>`;
  } catch (e) {
    document.getElementById("salle1").innerHTML = "<b>Erreur : ESP32 introuvable</b>";
  }
}
setInterval(update, 2000);
update();
</script>
</body>
</html>
```

### Annexe C – Configuration PlatformIO (`platformio.ini`)

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

board_build.filesystem = spiffs

lib_deps =
  me-no-dev/ESPAsyncWebServer @ ^1.2.3
  me-no-dev/AsyncTCP @ ^1.1.1
  adafruit/DHT sensor library
  bblanchon/ArduinoJson
```

### Annexe D – Glossaire

| Terme | Définition |
|---|---|
| **IoT** | Internet of Things – Internet des Objets |
| **ESP32** | Microcontrôleur Wi-Fi/Bluetooth de Espressif Systems |
| **DHT22** | Capteur numérique de température et d'humidité (précision ±0.5°C, ±2-5% HR) |
| **SPIFFS** | Serial Peripheral Interface Flash File System – système de fichiers embarqué |
| **API REST** | Architecture logicielle pour services web basée sur le protocole HTTP |
| **JSON** | JavaScript Object Notation – format léger d'échange de données |
| **PlatformIO** | Environnement de développement pour systèmes embarqués |
| **GPIO** | General Purpose Input/Output – broches d'entrée/sortie d'un microcontrôleur |
| **SPA** | Single Page Application – application web monopage |

---

*Rapport de stage rédigé par [Votre Nom Prénom] – [Année universitaire]*
