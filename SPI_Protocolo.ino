/*Referencias: 
- https://forum.arduino.cc/index.php?topic=616971.0/
- https://github.com/Jen456/TwoCardsRFID/blob/master/arduinoRFID.ino 
Cabe aclarar que estos fueron adaptados según las necesidades propias de este microproyecto.
*/

#include <SPI.h> //Libreria para incluir el protocolo SPI
#include <MFRC522.h> //Libreria para hacer uso de las tarjetas RFID

//Esclavo 1
#define RST1_PIN  9   //Pin 9 para el reset del RC522 #1 y #2
#define SS1_PIN  10   //Pin 10 para el SS (SDA) del RC522 #1
MFRC522 esclavo1(SS1_PIN, RST1_PIN); //Creamos el objeto para el RC522 #1

//Esclavo 2
#define SS2_PIN  8   //Pin 8 para el SS (SDA) del RC522 #2
MFRC522 esclavo2(SS2_PIN, RST1_PIN); //Creamos el objeto para el RC522 #2

void setup() {
  Serial.begin(9600); //Iniciamos la comunicación  serial
  SPI.begin();        //Iniciamos el Bus SPI
  
  esclavo1.PCD_Init(); // Iniciamos  el MFRC522 #1
  esclavo2.PCD_Init(); // Iniciamos  el MFRC522 #2
  
  Serial.println("Lectura del UID");
}

void loop() {
  tarjeta();
}

void tarjeta()
{ 
  // Reinicie el bucle si no hay una nueva tarjeta presente en el sensor / lector. Esto guarda todo el proceso cuando está inactivo.
  if ( esclavo1.PICC_IsNewCardPresent() || esclavo2.PICC_IsNewCardPresent() ) {
        Serial.println("Read...");}
  // Selecciona una de las cartas
  if ( esclavo1.PICC_ReadCardSerial()>0) {
                  for (byte i = 0; i < esclavo1.uid.size; i++) {
                          Serial.print(esclavo1.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(esclavo1.uid.uidByte[i], HEX);   
                  } 
                 Serial.println("Esclavo1");
                 Serial.println();
                  
                 
                  esclavo1.PICC_HaltA();  // Terminamos la lectura de la tarjeta  actual
    delay(1000);
  }
  if ( esclavo2.PICC_ReadCardSerial()>0) {
                     for (byte i = 0; i < esclavo2.uid.size; i++) {
                         Serial.print(esclavo2.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(esclavo2.uid.uidByte[i], HEX);   
                  } 
                  Serial.println("Esclavo2");
                  Serial.println();
                  
                  esclavo2.PICC_HaltA(); // Terminamos la lectura de la tarjeta  actual
    delay(1000);
  }
}
