String ultimaTargeta = "";
bool hayMensaje = false;
String comprovacio = "-1";
int comprovar_int;
#define TOPIC "test/topic"
#define ledVerde 2
#define ledRojo 15


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
      String payload = "{\"tagID\":\"" + tagID + "\"}";
      if (publishMessage(TOPIC, payload)) {
      } else {
        Serial.println("[MQTT] Envío fallido");
      }

      hayMensaje = false;
      
      while(!hayMensaje){
          leerMensaje();   
          delay(10);
      }

      Serial.println("TARGETA DETECTADA!");
      Serial.print("ID: ");
      Serial.println(tagID);
      comprovar_int = comprovacio.toInt();
      Serial.println(comprovar_int);
      
      if (comprovar_int == 1) {
         digitalWrite(ledVerde, HIGH);
         digitalWrite(ledRojo, LOW);
         delay(1000);
         digitalWrite(ledVerde, LOW);
      } else if (comprovar_int == 0) {
        digitalWrite(ledVerde, LOW);
        digitalWrite(ledRojo, HIGH);
        delay(2000);
        digitalWrite(ledRojo, LOW);
      } else {
        Serial.println("Programma apagat");
      }
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