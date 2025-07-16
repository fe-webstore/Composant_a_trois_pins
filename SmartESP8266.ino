// Inclusion des bibliothèques nécessaires
#include "SmartESP32Utils.h" // Assure-toi que cette lib est compatible ESP8266
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
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
const int PIN_POTENTIO = A0;
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
  potenPct = potenValue / 1023.0;
}

//🔢 Pourquoi 1023 ?
//L’ESP8266 possède un ADC (Analog to Digital Converter) de 10 bits, ce qui signifie qu’il convertit une tension analogique en une valeur numérique comprise entre :

//0 → pour 0V

//1023 → pour la tension maximale admissible (en général 1.0V ou 3.3V selon les modules)

//Donc, la formule :

//pourcentage = analogRead(A0) / 1023.0;
