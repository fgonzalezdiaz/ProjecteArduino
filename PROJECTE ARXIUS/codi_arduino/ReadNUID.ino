String ultimaTargeta = "";
int ComprovacioWifi = 0;
int ComprovacioAws = 0;
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
  clientLoop();
  delay(100);
  String tagID = "";
  if (CheckRFID(tagID)) {
    if (tagID != ultimaTargeta) {
      if(ComprovacioWifi == 1 && ComprovacioAws == 1){
        digitalWrite(ledVerde, HIGH);
        digitalWrite(ledRojo, LOW);
        delay(4500);
        digitalWrite(ledVerde, LOW);
      }
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