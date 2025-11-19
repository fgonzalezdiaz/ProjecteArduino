#ifndef RFID_FUNCTIONS_H
#define RFID_FUNCTIONS_H

#include <SPI.h>
#include <MFRC522.h>

// Definir els pins que utilitzarem
#define SS_PIN 5
#define RST_PIN 0

// Crear objecte per controlar el lector
MFRC522 rfid(SS_PIN, RST_PIN);

// Funció per inicialitzar el lector RFID
void SetupRFID() {
  // Iniciar SPI (comunicació amb el lector)
  SPI.begin(); 
  
  // Iniciar lector RFID
  rfid.PCD_Init(); 
  
  delay(100); // Esperar que s'inicialitzi
}

// Funció per comprovar si hi ha una targeta
bool CheckRFID(String &tag) {
  if (!rfid.PICC_IsNewCardPresent()) {
    return false;
  }
  
  if (!rfid.PICC_ReadCardSerial()) {
    return false;
  }
  
  tag = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      tag += "0";
    }
    tag += String(rfid.uid.uidByte[i], HEX);
  }
  
  tag.toUpperCase();
  rfid.PICC_HaltA();
  return true;
}

#endif