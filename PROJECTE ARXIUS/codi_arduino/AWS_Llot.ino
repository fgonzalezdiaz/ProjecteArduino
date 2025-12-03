#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

// Topic MQTT on es reb la resposta d’AWS
#define AWS_IOT_SUBSCRIBE_TOPIC "test/topic/response"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

String ultimoMensaje = "";
extern bool hayMensaje;
extern String comprovacio;



// CALLBACK: Quan arriba un missatge MQTT
void messageHandler(String &topic, String &payload) {
    // Si el missatge conté "tagID", vol dir que és un que ha enviat el dispositiu
    // No l'hem de processar com a resposta
    if (payload.indexOf("\"tagID\"") >= 0) {
        return; // Ignorar missatge propi
    }

    // Document JSON per deserialitzar el payload rebut
    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, payload);

    // Si la deserialització ha estat correcta, agafar el valor "message" 0 o 1
    if(!error){
      comprovacio = String(doc["message"].as<const char*>());   // 0 o 1
    }else {
      comprovacio = "-1"; // Missatge invàlid
    }
    
    // Indicar que ja tenim resposta
    hayMensaje = true;
}

void leerMensaje() {
  client.loop(); // Processa missatges entrants (només si s'anomena manualment)
}


// PUBLICAR MISSATGES MQTT
// Publica un missatge en el topic indicat.
// Retorna true si el publish ha anat bé.
bool publishMessage(const String &topic, const String &payload) {
  // Si estem desconnectats, intentar reconnectar abans de publicar
  if (!client.connected()) {
      Serial.println("[MQTT] Intentando reconexión antes de publicar...");
      reconnectMQTT();
      if (!client.connected()) {
          Serial.println("[MQTT] Fallo en la reconexión, no se pudo publicar.");
          return false;
      }
  }

  // Enviar el missatge
  bool ok = client.publish(topic.c_str(), payload.c_str());
  Serial.print("MQTT Publicado en "); Serial.print(topic); Serial.print(": "); Serial.println(payload);
  if (!ok) {
    Serial.println("MQTT publish() no ha podido enviar id targeta");
  }
  return ok;
}


// RECONEXIÓ AMB AWS MQTT
void reconnectMQTT() {
  // Si ja està connectat, no fer res
  if (client.connected()) {
    return; 
  }

  // Intentar connectar fins que AWS IoT respongui
  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(1000); // Espera 1 segundo entre intentos
  }

  // Un cop connectat, tornar a subscriure's al topic
  if (client.connected()) {
    Serial.println("\n Reconectado a AWS IoT!");
    client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
    Serial.println("⌛ ESPERANT TARGETA RFID...");
  } else {
    Serial.println("Fallo en la reconexión a AWS IoT.");
  }
}


// SETUP D'AWS IoT
void SetupAWS() {

  // Configurar WiFi en mode client
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi conectado!");

  // Carregar certificats de seguretat per AWS IoT
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Crear connexió MQTT segura contra AWS
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Assignar funció callback per missatges rebuts
  client.onMessage(messageHandler);

  Serial.println("Conectando a AWS IoT...");

  // Intentar connectar via MQTT
  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(500);
  }

  if (!client.connected()) {
    Serial.println("Tiempo de espera agotado conectando AWS IoT");
    return;
  }

  // Subscripció al topic de resposta
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("Suscrito al topic: " AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("Conectado a AWS IoT!");
}
