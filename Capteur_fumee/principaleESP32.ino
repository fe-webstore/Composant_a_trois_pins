// Inclusion des bibliothèques nécessaires
#include "SmartESP32Utils.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Informations d'identification WiFi
const char* ssid = "N1";
const char* password = "yannyann";

// Initialisation du serveur web asynchrone
AsyncWebServer server(80);
String lastCommand = "";
AutoUpdatePayload payload;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 500; // Intervalle de mise à jour en millisecondes

// Définition de la broche du capteur de fumée
const int PIN_SMOKE_SENSOR = 34; // GPIO34 par exemple

// Variables globales
int smokeRaw = 0;
float smokePct = 0.0;

void setup() {
  Serial.begin(115200); // Initialisation de la communication série
  connectToWiFi(ssid, password); // Connexion au réseau WiFi
  setupWebSocket(server, &lastCommand); // Configuration du WebSocket
}

void loop() {
  Smartcommande(); // Gestion des commandes intelligentes
  Autoupdate(); // Mise à jour automatique des données
}

void Smartcommande() {
  if (!lastCommand.isEmpty()) {
    lastCommand = "";
    sendAutoUpdate(payload);
    payload.notif = "false";
    payload.ia = "false";
  }
}

void Autoupdate() {
  if (millis() - lastUpdateTime > updateInterval) {
    lireFumee(); // Lecture de la valeur de fumée

    // Envoi des données
    payload.ecran1 = smokeRaw;
    payload.indicateur1 = smokePct;

    sendAutoUpdate(payload);
    lastUpdateTime = millis();
  }
}

void lireFumee() {
  // Lire la valeur brute du capteur de fumée
  smokeRaw = analogRead(PIN_SMOKE_SENSOR); 
  smokePct = (smokeRaw / 4095.0);
}

//🔢 Pourquoi des valeurs comme 4095 sur ESP32 ?
//Un convertisseur 12 bits donne des valeurs de 0 à 2¹² - 1 = 4095

//Donc :

//0 correspond à 0V
//4095 correspond à la tension maximale de référence, en général 3.3V

//C’est pourquoi dans ton code ESP32, tu as :
//smokePct = (smokeRaw / 4095.0);
