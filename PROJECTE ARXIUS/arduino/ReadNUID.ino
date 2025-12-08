String ultimaTargeta = "";
bool hayMensaje = false;
String comprovacio = "-1";
int comprovar_int;
#define TOPIC "test/topic"
#define ledVerde 2
#define ledRojo 15
extern void reconnectMQTT();


void setup() {
  Serial.begin(115200);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  
  // Asegurar que LEDs empiezan apagados
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, LOW);
  
  delay(3000);

  // Configuració inicial de Wifi, AWS i RFID
  SetupWifi();
  delay(3000);
  SetupAWS();
  delay(3000);
  Serial.println("INICIANT SISTEMA RFID...");
  Serial.println("⌛ ESPERANT TARGETA RFID...");
  SetupRFID(); 
}

void loop() {
  // Assegurar connexió MQTT
  reconnectMQTT();
  
  String tagID = "";
  // Comprovar si s'ha detectat una targeta RFID
  if (CheckRFID(tagID)) {
    // Evitar processar dues vegades la mateixa targeta
    if (tagID != ultimaTargeta) {
      
      // Crear i publicar el missatge JSON amb el tagID
      String payload = "{\"tagID\":\"" + tagID + "\"}";
      if (publishMessage(TOPIC, payload)) {
      } else {
        Serial.println("[MQTT] Envío fallido");
      }

      hayMensaje = false;
      
      // Esperar fins rebre resposta d'AWS
      while(!hayMensaje){
          leerMensaje();   
          delay(10);
      }

      Serial.println("TARGETA DETECTADA!");
      Serial.print("ID: ");
      Serial.println(tagID);

      // Convertir resposta del AWS del string a enter 
      comprovar_int = comprovacio.toInt();

      // Control dels LEDs segons resposta
      if (comprovar_int == 1) {
        Serial.println("✅ ACCES PERMÈS");
        digitalWrite(ledRojo, LOW);
        digitalWrite(ledVerde, HIGH);
      } else if (comprovar_int == 0) {
        Serial.println("❌ ACCES DENEGAT");
        digitalWrite(ledRojo, HIGH);
        digitalWrite(ledVerde, LOW);
      } else {
          Serial.println("❌ ACCES DENEGAT");
          digitalWrite(ledVerde, LOW);
          digitalWrite(ledRojo, HIGH);
      }

      // Guardar última targeta per evitar repeticions
      ultimaTargeta = tagID;

    } else {
      // Targeta repetida, bloquejar accés
      Serial.println("❌ ERROR : TARGETA REPETIDA");
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledRojo, HIGH);
    }
    
    // Parpelleig i reinicialització de LEDs
    delay(2000);
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, LOW);
    // Comprovar l'estat de la WiFi després d'un cicle
    CheckWifi();
    Serial.println("⌛ ESPERANT TARGETA RFID...");
  }
}