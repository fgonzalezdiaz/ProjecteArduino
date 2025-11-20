#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

// Topic MQTT donde recibes mensajes
#define AWS_IOT_SUBSCRIBE_TOPIC "test/topic"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

String ultimoMensaje = "";

// =============================
// CALLBACK: Cuando llega mensaje
// ============================= 

void messageHandler(String &topic, String &payload) {
    if (payload != "") {   // O también: if (payload != "")
      hayMensaje = true;
      Serial.print("Payload: ");
      Serial.println(payload);
    } else {
      hayMensaje = false;
    }
}

void leerMensaje() {
  client.loop(); // aquí procesas mensajes pendientes (usa si quieres procesar puntual)
}


// =============================
// PUBLICAR MENSAJES
// =============================
// Publica un mensaje en el topic indicado. Devuelve true si publish parece OK.
bool publishMessage(const String &topic, const String &payload) {
  if (!client.connected()) {
    Serial.println("MQTT No conectado, intentando reconectar...");
    if (!client.connect(THINGNAME)) { // Intentamos reconectar rápido 
      Serial.println("MQTT Reconexión fallida");
      return false;
    }
  }

  bool ok = client.publish(topic.c_str(), payload.c_str());
  if (!ok) {
    Serial.println("MQTT publish no acceptat");
  }
  return ok;
}


// =============================
// SETUP de AWS IoT
// =============================
void SetupAWS() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi conectado!");

  // Configurar certificados
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Configurar cliente MQTT contra AWS IoT
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Asignar callback de mensajes
  client.onMessage(messageHandler);

  Serial.println("Conectando a AWS IoT...");

  // Intentar conexión MQTT
  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(500);
  }

  if (!client.connected()) {
    Serial.println("❌ Tiempo de espera agotado conectando AWS IoT");
    return;
  }

  // Suscribirse al topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("📡 Suscrito al topic: " AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("✅ Conectado a AWS IoT!");
}

// =============================
// LOOP PRINCIPAL
// =============================
void clientLoop() {
  client.loop();   // <-- NECESARIO para recibir mensajes

  delay(10);
}

