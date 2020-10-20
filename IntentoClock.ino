const long INTERVALO_TIEMPO = 5000; //mili
int Reloj = D5; //configura velocidad de comunicación
int Envio_Raya = D6;
int Envio_Punto = D7;
bool validacion_envio = false; //si ya fue entregado el mensaje
int Estado_reloj;


//VARIABLES RELOJ
unsigned long tiempo_fin = 0;
unsigned long tiempo_inicio = millis();
int segundos; //leer el valor de entrada
int aleatorio = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(Reloj, OUTPUT);
  pinMode(Envio_Raya, OUTPUT);
  pinMode(Envio_Punto, OUTPUT);
  Serial.begin(9600);

}

void loop() {

  //Inicialiazamos el Contador del reloj
  Tiempo();
  if (detectaFlanco(Reloj) == -1) {
    Serial.println("flanco desendente");
      validacion_envio = true;
      if(validacion_envio == true){
        //aqui va todo los condicionales para saber como manejar los otros puertos
        }
  }
  if (detectaFlanco(Reloj) == 1) {
    Serial.println("Flanco Ascendentes");
    
    //punto:  Verde 1 Azul 0
    //Raya:   Verde 0 Azul 1
    //Espacio:Verde 1 Azul 1
    //¿Yo puedo Condicionar cada cuanto tiempo envio 1 o un 0 en cada pin?
    //si es asi, que tiempo seria el pruedente para hacer ese envio, teniendo en cuenta que tu ya entendiste que el arduino solo envia 
    //cuando el rejo detecte un flanco ascendente.
 
    //aqui es donde realmente envias los datos;
    /*digitalWrite(D6, Estado_Raya);
    digitalWrite(D7, Estado_Punto);

    digitalWrite(D6, HIGH);
    digitalWrite(D7, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
  */}
}
//Reloj
void Tiempo(){
  if (millis() - tiempo_fin) {
    tiempo_fin = millis();
    if (Estado_reloj == LOW) {
      Estado_reloj = HIGH;
    }
    else {
      Estado_reloj = LOW;
    }
  }
  digitalWrite(Reloj, Estado_reloj);
}

int detectaFlanco(int Reloj) {
  // 1= Flanco Ascendente, -1=Flanco Descendente, 0=No hay nada
  static boolean anterior_estado = digitalRead(Reloj);
  boolean estado = digitalRead(Reloj);

  if (anterior_estado != estado) {
    delay(100);
    if (estado == HIGH) {
      anterior_estado = estado;
      return 1;
    }
    else {
      anterior_estado = estado;
      return -1;
    }
  } else {
    return 0;
  }
}
