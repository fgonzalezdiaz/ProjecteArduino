#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

// Topic MQTT donde recibes mensajes
#define AWS_IOT_SUBSCRIBE_TOPIC "test/topic/response"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

String ultimoMensaje = "";
extern bool hayMensaje;
extern String comprovacio;

// =============================
// CALLBACK: Cuando llega mensaje
// ============================= 

void messageHandler(String &topic, String &payload) {
    // SI EL MENSAJE ES EL MISMO QUE HE ENVIADO, IGNORARLO
    if (payload.indexOf("\"tagID\"") >= 0) {
        // Mensaje que he enviado yo → ignorar
        return;
    }

    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if(!error){
      comprovacio = String(doc["message"].as<const char*>());   // 0 o 1
    }else {
      comprovacio = -1;
    }

    // Mensaje que viene REALMENTE de AWS
    Serial.print("Mensaje AWS: ");
    Serial.println(payload);

    hayMensaje = true;
}

void leerMensaje() {
  client.loop(); // aquí procesas mensajes pendientes (usa si quieres procesar puntual)
  if (!client.connected()) {
      Serial.println("MQTT desconectado, reconectando...");
      client.connect(THINGNAME);
      client.subscribe("test/topic/response");
  }
}


// =============================
// PUBLICAR MENSAJES
// =============================
// Publica un mensaje en el topic indicado. Devuelve true si publish parece OK.
bool publishMessage(const String &topic, const String &payload) {
  if (!client.connected()) {
    Serial.println("MQTT No conectado, intentando reconectar...");
    // Intentar reconectar rápido (puedes ajustar/reemplazar con tu propia lógica)
    if (!client.connect(THINGNAME)) {
      Serial.println("MQTT Reconexión fallida");
      return false;
    }
  }

  bool ok = client.publish(topic.c_str(), payload.c_str());
  Serial.print("MQTT Publicado en "); Serial.print(topic); Serial.print(": "); Serial.println(payload);
  if (!ok) {
    Serial.println("MQTT publish() no ha podido enviar id targeta");
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
