String ultimaTargeta = "";
bool hayMensaje = false;
#define ledVerde 2
#define ledRojo 15
#define TOPIC "test/topic"

void setup() {
  Serial.begin(115200);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  delay(3000);
  SetupWifi();
  delay(3000);
  SetupAWS();
  delay(3000);
  Serial.println("INICIANT SISTEMA RFID...");
  Serial.println("ESPERANT TARGETA RFID...");
  SetupRFID(); 
}

void loop() {

  String tagID = "";
  if (CheckRFID(tagID)) {
    if (tagID != ultimaTargeta) {
      // Publicar el ID de la tarjeta al topic MQTT (ejemplo JSON)
      String payload = "{\"ID\":\"" + tagID + "\"}";
      if (publishMessage(TOPIC, payload)) {
        Serial.println("Missatge Enviat");
      } else {
        Serial.println("Error : Missatge no enviat");
      }
      while(hayMensaje == false){
          leerMensaje();
      }
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledRojo, LOW);
      delay(1000);
      digitalWrite(ledVerde, LOW);
      Serial.println("TARGETA DETECTADA!");
      Serial.print("ID: ");
      Serial.println(tagID);
      ultimaTargeta = tagID;
    }else{
      Serial.println("ERROR : TARGETA REPETIDA");
      Serial.println("ESPERANT NOVA TARGETA RFID...");
      digitalWrite(ledRojo, HIGH);
      digitalWrite(ledVerde, LOW);
      delay(2500);
      digitalWrite(ledRojo, LOW);
    }
    delay(1000);
    CheckWifi();
    Serial.println("ESPERANT TARGETA RFID...");
  }
}