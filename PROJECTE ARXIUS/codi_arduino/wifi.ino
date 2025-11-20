#include <WiFi.h>
const char *password = "francisco";
const char *ssid = "iPhone de Francisco";

void SetupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("CONNECTANT A WIFI");

  unsigned long start = millis();
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100); // Muy importante para el watchdog
    if (millis() - start > 10000) { // Timeout 10s
      Serial.println("\nNo es pot connectar a WiFi, continuant sense WiFi");
      return; // Sale del setup sin bloquear
    }
  }
  Serial.println("\nPlaca connectada!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}


void CheckWifi() {

  // Serial.print("COMPROBANT CONNEXIÓ A WIFI ..."); 
  //delay(1000);
  unsigned long started = millis();
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100); // Muy importante para el watchdog
    if (millis() - started > 10000) { // Timeout 10s
      Serial.println("S'ha perdut la connexió al Wifi, continuant sense WiFi");
      return; // Sale del setup sin bloquear
    }
  }
  Serial.println("PLACA SEGUEIX CONECTADA AL WIFI!");
}