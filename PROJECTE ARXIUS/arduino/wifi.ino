#include <WiFi.h>
const char *password = "francisco";
const char *ssid = "iPhone de Francisco";

// CONFIGURACIÓ INICIAL DE LA WIFI
void SetupWifi() {

  // Posar el WiFi en mode client
  WiFi.mode(WIFI_STA);
  // Intentar connectar-se a la xarxa WiFi
  WiFi.begin(ssid, password);

  Serial.print("CONNECTANT A WIFI");

  unsigned long start = millis();

  // Esperar la connexió amb timeout de 10 segons
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100); 
    if (millis() - start > 10000) { 
      Serial.println("\nNo es pot connectar a WiFi, continuant sense WiFi");
      return; // Sortir sense bloquejar el sistema
    }
  }
  Serial.println("\nPlaca connectada!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// COMPROVAR SI LA WIFI SEGUEIX CONNECTADA
void CheckWifi() {

  // Timer per controlar timeout
  unsigned long started = millis();

  // Esperar que torni la connexió si s’ha perdut
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100); 
    if (millis() - started > 10000) {
      Serial.println("S'ha perdut la connexió al Wifi, continuant sense WiFi");
      return;  // No bloquejar el programa
    }
  }

  // Si segueix connectada, informar
  Serial.println("PLACA SEGUEIX CONECTADA AL WIFI!");
}