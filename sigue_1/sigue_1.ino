/*
  programa de prueba de los sensores infrarrojos
  conectar 2 sensores y calibrar con un potenciometro
*/
#define  enMotIzq 5
#define  senMotIzq1 2
#define  senMotIzq2 4
//declaracion de pines para el motor derecho
#define  enMotDer 6
#define  senMotDer1 7
#define  senMotDer2 8


//SENSORES

#define LED_IZQ 12
#define LED_DER 11
#define UMBRAL 200

#define IZQ_ES_NEGRO (sensor_izq > UMBRAL)
#define DER_ES_NEGRO (sensor_der > UMBRAL)

int sensor_izq;  //variable que almacena el valor leido del sensor izquierdo
int sensor_der;  // '' derecho
//int UMBRAL;      //variable leida del potenciometro para calibrar

//--------------------------------------------

// MOTORES


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pines de control del driver de motores 
  pinMode(enMotIzq, OUTPUT);
  pinMode(senMotIzq1, OUTPUT);
  pinMode(senMotIzq2, OUTPUT);
  pinMode(enMotDer, OUTPUT);
  pinMode(senMotDer1, OUTPUT);
  pinMode(senMotDer2, OUTPUT);
  
  //habilitar motores
  Activar();
  
  pinMode(LED_IZQ, OUTPUT);
  pinMode(LED_DER, OUTPUT);
}

void loop() {
  //leer umbral
  //UMBRAL = analogRead(A5);
  delay(5);
  // put your main code here, to run repeatedly:
  sensor_izq = analogRead(A0);
  delay(5);
  sensor_der = analogRead(A2);
  Serial.println("sen_izq  -- sen_der");
  Serial.print(sensor_izq);
  Serial.print("      ");
  Serial.println(sensor_der);
  
  if ( sensor_izq > UMBRAL ) digitalWrite(LED_IZQ,1);
  else digitalWrite(LED_IZQ,0);
  if ( sensor_der > UMBRAL ) digitalWrite(LED_DER,1);
  else digitalWrite(LED_DER,0);
  
  if(!IZQ_ES_NEGRO && !DER_ES_NEGRO)
  {
    Adelante(1);
  }  
  else if(!IZQ_ES_NEGRO && DER_ES_NEGRO)
  {
    Derecha(1);
  }  
  else if(IZQ_ES_NEGRO && !DER_ES_NEGRO)
  {
    Izquierda(1);
  }  
  else if(IZQ_ES_NEGRO && DER_ES_NEGRO)
  {
    //Detener();
    Adelante(1);
  }  
  else Detener();
  
}


void Adelante(int segundos)
{
  //escribo las salidas para mover el motor
  //MOTOR IZQ ADELANTE
  Serial.println("adelante!");
  digitalWrite(senMotIzq1, 1);
  digitalWrite(senMotIzq2, 0);
  //MOTOR DER ADELANTE
  digitalWrite(senMotDer1, 1);
  digitalWrite(senMotDer2, 0);
  delay(segundos * 1000);
}


void Atras(int segundos)
{
  //escribo las salidas para mover el motor
  //MOTOR IZQ Atras
  Serial.println("atras!");
  digitalWrite(senMotIzq1, 0);
  digitalWrite(senMotIzq2, 1);
  //MOTOR DER Atras
  digitalWrite(senMotDer1, 0);
  digitalWrite(senMotDer2, 1);
  delay(segundos * 1000);
}


void Izquierda(int segundos)
{
  //escribo las salidas para mover el motor
  //MOTOR IZQ ADELANTE
  Serial.println("izquierda!");
  digitalWrite(senMotIzq1, 1);
  digitalWrite(senMotIzq2, 0);
  //MOTOR DER detenido
  digitalWrite(senMotDer1, 1);
  digitalWrite(senMotDer2, 1);
  delay(segundos * 1000);
}


void Derecha(int segundos)
{
  //escribo las salidas para mover el motor
  //MOTOR IZQ detenido
  Serial.println("derecha!");
  digitalWrite(senMotIzq1, 1);
  digitalWrite(senMotIzq2, 1);
  //MOTOR DER ADELANTE
  digitalWrite(senMotDer1, 1);
  digitalWrite(senMotDer2, 0);
  delay(segundos * 1000);
}


void Detener(void)
{
  //escribo las salidas para mover el motor
  //MOTOR IZQ ADELANTE
  
  Serial.println("detenido!");
  digitalWrite(enMotIzq, 0);
  digitalWrite(enMotDer, 0);
  
}

void Activar(void)
{
  //escribo las salidas para mover el motor
  //MOTOR IZQ ADELANTE
  digitalWrite(enMotIzq, 1);
  digitalWrite(enMotDer, 1);
  
}
