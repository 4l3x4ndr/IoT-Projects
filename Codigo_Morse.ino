//https://forum.arduino.cc/index.php?topic=430364.0

const int Led = 13;
int Punto = 200;   // Valor duracion del punto.(la Raya es Punto * 3,tiempo entre . y - (200) espacio entre letras (-),espacio entre palabras (Punto * 7)

//Array con datos ya escritos ,que son la traducuin del abecedario en codigo morse.
char* MorseABC[] = {

".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..",      //A asta L
"--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--", // L asta Y
"--.."};                                                   //Z

//Array con datos ya escritos, que son los numero en modo morse
char* Numeros[] = {

  "-----",".----","..---","....-",".....","-....","--...","---..","----."
};

void setup()
{
  pinMode(Led, OUTPUT);
  Serial.begin(9600);
}

void loop()
{

  char caracter;            //Variable que almacena el caracter dado por el mensejae enviado.

  if(Serial.available())            //Comando de lectura de USB por monitor.
  {
    caracter = Serial.read();       //Asigna la lectura del USB a el caracter.

    //Explicacion del codigo ASCII , todo caracter o numero en programacion , tiene un valor numerico , ejemplo el abecedario en minuscula va desde el nÂº 97 que es la 'a' al 122 que es la 'z'.
    //Por eso aqui hacemos restas de letras. y asi haberiguamos la posicion del caracter itroducido. ejemplo : metemos la 'c'  entoces 'c' (codigo ASCII = 99) - 'a'(97) = 2
    // este resultado es la posicion del 'c' en la tabla que es "-.-."

    if(caracter >= 'a' && caracter <= 'z' )
    {
      RecoridoCadena(MorseABC[caracter - 'a']);
    }
    else if(caracter >= 'A' && caracter <= 'Z' )
    {
      RecoridoCadena(MorseABC[caracter - 'A']);
    }
    else if(caracter >= '0' && caracter <= '9' )
    {
      RecoridoCadena(MorseABC[caracter - '0']);
    }
    else if(caracter == ' ' )
    {
      delay(Punto * 7);
    }
  }

}

//Funcion que recorre las posiciones de las cadenas usando como variable un puntero de tipo char.
void RecoridoCadena(char* posicion)
{
  int i = 0;                //Variable usada como contador , para pasar por las posiciones de las cadenas
  while(posicion[i] != NULL)        //Bucle while (Traducion: Mientras(la posicion NÂº tal,no sea igual a NULL))
  {
    EncendidoLED(posicion[i]);      //Invocamos la funcios (para la posicion NÂº tal.Ya la funcion invocada determina que tiene que hacer.
    i++;                //Aumento de la la variable i, este aumento es = i + 1, en c++ ,++ significa +1.
  }
  delay(Punto * 3);           //Espera entre palabras el valor de una raya que es el multiplo de 3 de un Punto.
}


//Funcion que enciende el LED con la duracion del valor del punto y el valor de la raya ,
//termina la funcion y apaga el LED durante la duracion del punto que es el espacio entre letras
void EncendidoLED(char caracter)
{
  digitalWrite(Led,HIGH);         //Enciende el LED,
  if(caracter == '.')           //Condicion que se traduce como si caracter es igual a . el LED se enciende la duracion de la variable Punto
  {
    delay(Punto);           //Duracion.
  }
  else                  //Lo contrario de la IF , Seria si no , el LED se enciende la duracion de la variable Punto * 3,Que es el valor de la Raya.
  {
    delay(Punto * 3);
  }
  digitalWrite(Led,LOW);          //Apaga el LED
  delay(Punto);             //Duracion de VAriable.
}
