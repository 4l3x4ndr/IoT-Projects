/*Código basado en los códigos de:
  Electrotec https://www.youtube.com/watch?v=FwPhzd8r_JY&feature=youtu.be
  Bitwise Ar https://github.com/bitwiseAr/Curso-Arduino-desde-cero/blob/master/Capitulo40/Capitulo40-Programa2.txt

  Cabe aclarar que estos fueron adaptados según las necesidades propias de este microproyecto.
*/

#include <SPI.h>      // incluye libreria bus SPI
#include <MFRC522.h>      // incluye libreria especifica para MFRC522

#define RST_PIN  9      // constante para referenciar pin de reset
#define SS_PIN  10      // constante para referenciar pin de slave select

#define greenLed 7  
#define yellowLed 6    
#define redLed   5  


MFRC522 mfrc522(SS_PIN, RST_PIN);  

/*Creo que no es necesario el DO While, porque si llega acá es porque pasó por el Loop y leyó una nueva tarjeta... Falta es toda la lógica de lo del aleatorio para mostrar las imágenes
 * que no sé si iría en un programa distinto o acá mismo porque estoy un poco perdida en el contexto general. En caso de que vaya acá, con el random que me habías mostrado, entonces 
 * debes usar mucho las banderas.
*/

byte ReadUID[4];         // crea array para almacenar el UID leido

byte Q1Correct[4]= {0xA9, 0x40, 0xEC, 0x5A} ;    // UID de tarjeta-pregunta 1 correcta
byte Q1Wrong[4]= {0x89, 0x74, 0xE1, 0x5A} ;    // UID de tarjeta-pregunta 1 incorrecta

byte Q2Correct[4]= {0x89, 0x97, 0xE0, 0x5A} ;    // UID de tarjeta-pregunta 2 correcta
byte Q2Wrong[4]= {0x29, 0xD8, 0xB0, 0x59} ;    // UID de tarjeta-pregunta 2 incorrecta

byte Q3Correct[4]= {0x79, 0xE5, 0xC8, 0x5A} ;    // UID de tarjeta-pregunta 3 correcta
byte Q3Wrong[4]= {0xD9, 0xCB, 0xBD, 0x5A} ;    // UID de tarjeta-pregunta 3 incorrecta

byte Q4Correct[4]= {0x19, 0xCD, 0xB7, 0x59} ;  // UID de tarjeta-pregunta 4 correcta
byte Q4Wrong[4]= {0x69, 0x63, 0xB8, 0x5A} ;    // UID de tarjeta-pregunta 4 incorrecta

byte Q5Correct[4]= {0xB9, 0xE3, 0x7E, 0xE5} ;  // UID de tarjeta-pregunta 5 correcta
byte Q5Wrong[4]= {0x49, 0x17, 0x47, 0xE8} ;    // UID de tarjeta-pregunta 5 incorrecta

bool flagWaiting=false;

int question=0;
int contador=0;

void setup() {
  pinMode(greenLed,OUTPUT);
  pinMode(yellowLed,OUTPUT);  
  pinMode(redLed,OUTPUT);

  Serial.begin(9600); // inicializa comunicacion por monitor serie a 9600 bps
  SPI.begin();        // inicializa bus SPI
  mfrc522.PCD_Init(); // inicializa modulo lector
  Serial.println("MEMORY GAME: Based on the images seen above, answer the following questions.");      
}

void loop() {
if(!flagWaiting){ //Si no ha generado la pregunta aleatoria
    
    question = question+1;//Generación del # de la respuesta;
   // Serial.println(question);
    
    switch (question) {
      case 1:
        Serial.println("1. What is the name of the street you saw?");
      break;
      case 2:
        Serial.println("2. What is the button were they pushing in the elevator?");
      break;
      case 3:
        Serial.println("3. What is the color of the book on the desk?");
      break;
      case 4:
        Serial.println("4. What was the bus number?");
      break;
      case 5:
        Serial.println("5. The children were…?");
      break;
       case 6:
        Serial.println("RESULTS");
        Serial.print("\t");   
        Serial.print("You got ");
        Serial.print(contador);
        Serial.print(" answers out of 5 correct");

         if (contador<3)
      {
        Serial.print("\t");  
        Serial.println("Try again");
      }
      else if (contador=3)
      {
        Serial.print("\t");   
        Serial.println("You have an overage memory");
      } 
       else if (contador>3)
      {
        Serial.print("\t");   
        Serial.println("You have an excellent memory");
      } 
      break;
    }
    flagWaiting = true;
  }
  
else{ //Es decir, si ya generó la pregunta y está esperando a que responda con las tarjetas. 

  if ( ! mfrc522.PICC_IsNewCardPresent()){   // si no hay una tarjeta presente
    //Serial.println("Test: There is no Card");
    return;           // retorna al loop esperando por una tarjeta
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial())     // si no puede obtener datos de la tarjeta
    return;           // retorna al loop esperando por otra tarjeta

  for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
    ReadUID[i]=mfrc522.uid.uidByte[i];     // almacena en array el byte del UID leido      
    }
      
  switch (question) {
    case 1:
      // Question 1
      if (ComparaUID(ReadUID, Q1Correct))
      {
        Success();
        Serial.print("\t");   
        Serial.println("Correct");
        contador=contador+1;
        flagWaiting = false; // Generar otra respuesta
      }
      else if (ComparaUID(ReadUID, Q1Wrong))
      {
        Error();
        Serial.print("\t");   
        Serial.println("Wrong");
        flagWaiting = false; // Generar otra respuesta
      } 
      else
      {
        Unidentified();
        Serial.print("\t");   
        Serial.println("This card is not part of the possible answers to this question");
        flagWaiting = true; 
      }
    break;
    
    case 2:
      // Question 2
      if (ComparaUID(ReadUID, Q2Correct))
      {
        Success();
        Serial.print("\t");   
        Serial.println("Correct");
        contador=contador+1;
        flagWaiting = false; // Generar otra respuesta
      }
      else if (ComparaUID(ReadUID, Q2Wrong))
      {
        Error();
        Serial.print("\t");   
        Serial.println("Wrong");
        flagWaiting = false; // Generar otra respuesta
      }
      else
      { 
        Unidentified();
        Serial.print("\t");   
        Serial.println("This card is not part of the possible answers to this question");
        flagWaiting = true; 
      }
    break;
    
    case 3:
      // Question 3
      if (ComparaUID(ReadUID, Q3Correct))
      {
        Success();
        Serial.print("\t");   
        Serial.println("Correct");
        contador=contador+1;
        flagWaiting = false; // Generar otra respuesta
      }
      else if (ComparaUID(ReadUID, Q3Wrong))
      {
        Error();
        Serial.print("\t");   
        Serial.println("Wrong");
        flagWaiting = false; // Generar otra respuesta
      }
     else
      { 
        Unidentified();
        Serial.print("\t");   
        Serial.println("This card is not part of the possible answers to this question");
        flagWaiting = true; 
      }
    break;
    
    case 4:
      // Question 4
      if (ComparaUID(ReadUID, Q4Correct))
      {
        Success();
        Serial.print("\t");   
        Serial.println("Correct");
        contador=contador+1;
        flagWaiting = false; // Generar otra respuesta
      }
      else if (ComparaUID(ReadUID, Q4Wrong))
      {
        Error();
        Serial.print("\t");   
        Serial.println("Wrong");
        flagWaiting = false; // Generar otra respuesta
      }
     else
      {
        Unidentified();
        Serial.print("\t");
        Serial.println("This card is not part of the possible answers to this question");
        flagWaiting = true; 
      }
    break;
    
    case 5:
      // Question 5
      if (ComparaUID(ReadUID, Q5Correct))
      {
        Success();
        Serial.print("\t");   
        Serial.println("Correct");
        contador=contador+1;
        flagWaiting = false; // Generar otra respuesta
      }
      else if (ComparaUID(ReadUID, Q5Wrong))
      {
        Error();
        Serial.print("\t");   
        Serial.println("Wrong");
        flagWaiting = false; // Generar otra respuesta
      }
      else
      {
        Unidentified();
        Serial.print("\t");   
        Serial.println("This card is not part of the possible answers to this question");
        flagWaiting = true; 
      }
    break;

// Resultados
    case 6:
      flagWaiting = false; 
    break;
  }
 }
 mfrc522.PICC_HaltA();     // detiene comunicacion con tarjeta
}

boolean ComparaUID(byte read[],byte card[]) // funcion ComparaUID
{
  for (byte i=0; i < mfrc522.uid.size; i++){    // bucle recorre de a un byte por vez el UID
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
