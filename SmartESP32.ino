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

// Définition de la broche du potentiomètre
const int PIN_POTENTIO = 35;
int potenValue = 0;
float potenPct = 0.0;

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
  // Vérifie s'il y a une nouvelle commande
  if (!lastCommand.isEmpty()) {
    lastCommand = "";
    sendAutoUpdate(payload);
    payload.notif = "false";
    payload.ia = "false";
  }
}

void Autoupdate() {
  // Met à jour les données à intervalles réguliers
  if (millis() - lastUpdateTime > updateInterval) {
    lirePoten(); // Lecture de la valeur du potentiomètre

    // Envoi des données lues
    payload.ecran1 = potenValue;
    payload.indicateur1 = potenPct;

    sendAutoUpdate(payload);
    lastUpdateTime = millis(); // Met à jour le temps de la dernière mise à jour
  }
}



void lirePoten() {
  // Lit la valeur du potentiomètre et calcule le pourcentage
  potenValue = analogRead(PIN_POTENTIO);
  potenPct = (potenValue / 4095.0);
}




//🔢 Pourquoi des valeurs comme 4095 sur ESP32 ?
//Un convertisseur 12 bits donne des valeurs de 0 à 2¹² - 1 = 4095

//Donc :

//0 correspond à 0V

//4095 correspond à la tension maximale de référence, en général 3.3V

//C’est pourquoi dans ton code ESP32, tu as :

//potenPct = (potenValue / 4095.0);
