/*Código basado en los códigos de:
 https://github.com/4l3x4ndr/IoT-Projects/blob/master/TestMemoria_Final.ino
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

#define greenLed 7  
#define yellowLed 6    
#define redLed   5  

byte ReadUID[4];         // crea array para almacenar el UID leido

byte 1Concept[4]= {0xA9, 0x40, 0xEC, 0x5A} ;    // UID de tarjeta-concepto 1
byte 1Image[4]= {0x89, 0x74, 0xE1, 0x5A} ;    // UID de tarjeta-imagen 1

byte 2Concept[4]= {0x89, 0x97, 0xE0, 0x5A} ;    // UID de tarjeta--concepto 2
byte 2Image= {0x29, 0xD8, 0xB0, 0x59} ;    // UID de tarjeta-imagen 2

byte 3Concept[4]= {0x79, 0xE5, 0xC8, 0x5A} ;    // UID de tarjeta-concepto 3
byte 3Image[4]= {0xD9, 0xCB, 0xBD, 0x5A} ;    // UID de tarjeta-imagen 3

bool flagWaiting=false;

int question=0;
int contador=0;

void setup() {
  pinMode(greenLed,OUTPUT);
  pinMode(yellowLed,OUTPUT);  
  pinMode(redLed,OUTPUT);

  Serial.begin(9600); // inicializa comunicacion por monitor serie a 9600 bps
  SPI.begin();        // inicializa bus SPI
  
  esclavo1.PCD_Init(); // Iniciamos  el MFRC522 #1
  esclavo2.PCD_Init(); // Iniciamos  el MFRC522 #2
  
  Serial.println("LAS PISTAS DE BLUE");
  Serial.println();
  Serial.println ("Imagine that you are an explorer and you are trying to find some wild animals in their habitat, but for this you must know what to call them in English, Spanish and know what they look like, are you ready for the adventure?");
  Serial.println ("Match the cards that correspond to the translation in Spanish and the image of the concept that will be given to you in English in the statement.");    
  Serial.println();  
}

void loop() {
if(!flagWaiting){ //Si no ha generado la pregunta aleatoria
    
    question = question+1;//Generación del # de la respuesta;
   // Serial.println(question);
    
    switch (question) {
      case 1:
        Serial.println("Which cards represent a LION?");
      break;
      case 2:
        Serial.println("Which cards represent an ELEPHANT?");
      break;
      case 3:
        Serial.println("Which cards represent a SNAKE?");
      break;
      case 4:
        Serial.println("RESULTS");
        Serial.print("\t");   
        Serial.print("You got ");
        Serial.print(contador);
        Serial.print(" answers out of 3 correct");
         if (contador<2)
      {
        Serial.print("\t");  
        Serial.println("Try again");
      }
      else if (contador=2)
      {
        Serial.print("\t");   
        Serial.println("You can do better");
      } 
       else if (contador>3)
      {
        Serial.print("\t");   
        Serial.println("Congratulations!");
      } 
      break;
    }
    flagWaiting = true;
  }
  
else{ //Es decir, si ya generó la pregunta y está esperando a que responda con las tarjetas. 

  if ( esclavo1.PICC_IsNewCardPresent() || esclavo2.PICC_IsNewCardPresent() ) {
        Serial.println("Read...");}
  // Selecciona una de las cartas
  if ( esclavo1.PICC_ReadCardSerial()>0) {
                  for (byte i = 0; i < esclavo1.uid.size; i++) {
                      ReadUID[i]=esclavo1.uid.uidByte[i];     // almacena en array el byte del UID leido   
                  } 
                 //Serial.println("Esclavo1");
  }
  if ( esclavo2.PICC_ReadCardSerial()>0) {
                     for (byte i = 0; i < esclavo2.uid.size; i++) {
                        ReadUID[i]=esclavo2.uid.uidByte[i];     // almacena en array el byte del UID leido  
                  } 
                  //Serial.println("Esclavo2");
 }    
 
  switch (question) {
    case 1:
      // Question 1
      if (ComparaUID_Esclavo1(ReadUID, 1Concept) && ComparaUID_Esclavo2(ReadUID, 1Image))
      {
        Success();
        Serial.print("\t");   
        Serial.println("Very good!");
        contador=contador+1;
        flagWaiting = false; // Generar otra respuesta
      }
      else if (!ComparaUID_Esclavo1(ReadUID, 1Concept) && !ComparaUID_Esclavo2(ReadUID, 1Image))
      {
        Error();
        Serial.print("\t");   
        Serial.println("Maybe the next time... ");
        flagWaiting = false; // Generar otra respuesta
      } 
      else if (!ComparaUID_Esclavo1(ReadUID, 1Concept) || !ComparaUID_Esclavo2(ReadUID, 1Image))
      {
        Unidentified();
        Serial.print("\t");   
        Serial.println("Are you sure? Something looks strange...");
        contador=contador+0.5;
        flagWaiting = false; // Generar otra respuesta
      } 
      else
      {
        Serial.print("\t");   
        Serial.println("This card is not part of the possible answers");
        flagWaiting = true; 
      }
    break;
    
    case 2:
      // Question 2
     if (ComparaUID_Esclavo1(ReadUID, 2Concept) && ComparaUID_Esclavo2(ReadUID, 2Image))
      {
        Success();
        Serial.print("\t");   
        Serial.println("Very good!");
        contador=contador+1;
        flagWaiting = false; // Generar otra respuesta
      }
      else if (!ComparaUID_Esclavo1(ReadUID, 2Concept) && !ComparaUID_Esclavo2(ReadUID, 2Image))
      {
        Error();
        Serial.print("\t");   
        Serial.println("Maybe the next time... ");
        flagWaiting = false; // Generar otra respuesta
      } 
      else if (!ComparaUID_Esclavo1(ReadUID, 2Concept) || !ComparaUID_Esclavo2(ReadUID, 2Image))
      {
        Unidentified();
        Serial.print("\t");   
        Serial.println("Are you sure? Something looks strange...");
        contador=contador+0.5;
        flagWaiting = false; // Generar otra respuesta
      } 
      else
      {
        Serial.print("\t");   
        Serial.println("This card is not part of the possible answers");
        flagWaiting = true; 
      }
    break;
    
    case 3:
      // Question 3
     if (ComparaUID_Esclavo1(ReadUID, 3Concept) && ComparaUID_Esclavo2(ReadUID, 3Image))
      {
        Success();
        Serial.print("\t");   
        Serial.println("Very good!");
        contador=contador+1;
        flagWaiting = false; // Generar otra respuesta
      }
      else if (!ComparaUID_Esclavo1(ReadUID, 3Concept) && !ComparaUID_Esclavo2(ReadUID, 3Image))
      {
        Error();
        Serial.print("\t");   
        Serial.println("Maybe the next time... ");
        flagWaiting = false; // Generar otra respuesta
      } 
      else if (!ComparaUID_Esclavo1(ReadUID, 3Concept) || !ComparaUID_Esclavo2(ReadUID, 3Image))
      {
        Unidentified();
        Serial.print("\t");   
        Serial.println("Are you sure? Something looks strange...");
        contador=contador+0.5;
        flagWaiting = false; // Generar otra respuesta
      } 
      else
      {
        Serial.print("\t");   
        Serial.println("This card is not part of the possible answers");
        flagWaiting = true; 
      }
    break;
    
 // Resultados
    case 4:
      flagWaiting = false; 
    break;
  }
 }
 esclavo1.PICC_HaltA(); // Terminamos la lectura de la tarjeta RC522 #1
 esclavo2.PICC_HaltA(); // Terminamos la lectura de la tarjeta RC522 #2
}

boolean ComparaUID_Esclavo1(byte read[],byte card[]) // funcion ComparaUID
{
  for (byte i=0; i < esclavo1.uid.size; i++){    // bucle recorre de a un byte por vez el UID
  if(read[i] != card[i])  // si byte de UID leido es distinto a usuario
    return false;         // retorna falso
  }
  return true;           // si los 4 bytes coinciden retorna verdadero
}

boolean ComparaUID_Esclavo2(byte read[],byte card[]) // funcion ComparaUID
{
  for (byte i=0; i < esclavo2.uid.size; i++){    // bucle recorre de a un byte por vez el UID
  if(read[i] != card[i])  // si byte de UID leido es distinto a usuario
    return false;         // retorna falso
  }
  return true;           // si los 4 bytes coinciden retorna verdadero
}

void Success(){
  digitalWrite(greenLed,HIGH);
  delay(1000);
  digitalWrite(greenLed,LOW);
}

void Error(){  
  digitalWrite(redLed,HIGH);
  delay(1000);
  digitalWrite(redLed,LOW);
 }   

void Unidentified(){  
   digitalWrite(yellowLed,HIGH);
   delay(1000);
   digitalWrite(yellowLed,LOW);
 }   
