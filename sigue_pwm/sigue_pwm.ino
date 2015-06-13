// DEFINICION DE VARIABLES
int sens_izq;
int sens_der;

const int umbral = 500;
//pines para el puente h

//motor izquierdo
const int EN_M_IZQ = 5;
const int S1_M_IZQ = 2;
const int S2_M_IZQ = 4;
//motor derecho
const int EN_M_DER = 6;
const int S1_M_DER = 7;
const int S2_M_DER = 8;

const int LED = 13;


void setup() {
  // put your setup code here, to run once:
  //configurar pines de entrada y salida digital
  //pinMode(EN_M_IZQ, OUTPUT);
  pinMode(S1_M_IZQ, OUTPUT);
  pinMode(S2_M_IZQ, OUTPUT);
  //pinMode(EN_M_DER, OUTPUT);
  pinMode(S1_M_DER, OUTPUT);
  pinMode(S2_M_DER, OUTPUT);
  
  //iniciamos el puerto serial
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //SENSADO
  sens_izq = analogRead(A0);    //sensor izquierdo
  sens_der = analogRead(A1);    //sensor derecho
  
  //IMPRIMIR
  //Serial.println("IZQ -- DER");
  Serial.print(sens_izq);
  Serial.print("    ");
  Serial.println(sens_der);
  //thresholding
  int izq_negro = sens_izq > umbral;
  int der_negro = sens_der > umbral;
  //DECISION
  if(!izq_negro && !der_negro)
  {
    //adelante
    adelante(110);
    
  }
  else if(izq_negro && !der_negro)
  {
    //izquierda
    izquierda(160);
  }
  else if(!izq_negro && der_negro)
  {
    //der
    derecha(160);
  }
  else if(izq_negro && der_negro)
  {
    //detener
    detener();
  }
  else detener();//detener
  
  
}

void adelante(int vel)
{
  //motor Izq
  analogWrite(EN_M_IZQ, vel);    //velocidad
  digitalWrite(S1_M_IZQ, 1);
  digitalWrite(S2_M_IZQ, 0);
  //motor der
  analogWrite(EN_M_DER, vel);
  digitalWrite(S1_M_DER, 1);
  digitalWrite(S2_M_DER, 0);
  
}
void izquierda(int vel)
{
  //motor Izq
  analogWrite(EN_M_IZQ, 0);
  digitalWrite(S1_M_IZQ, 1);
  digitalWrite(S2_M_IZQ, 0);
  //motor der
  analogWrite(EN_M_DER, vel);
  digitalWrite(S1_M_DER, 1);
  digitalWrite(S2_M_DER, 0);
  
}
void derecha(int vel)
{
  //motor Izq
  analogWrite(EN_M_IZQ, vel);
  digitalWrite(S1_M_IZQ, 1);
  digitalWrite(S2_M_IZQ, 0);
  //motor der
  analogWrite(EN_M_DER, 0);
  digitalWrite(S1_M_DER, 1);
  digitalWrite(S2_M_DER, 0);
  
 
}
void detener()
{
  //motor Izq
  analogWrite(EN_M_IZQ, 0);
  
  //motor der
  analogWrite(EN_M_DER, 0);
  
  
}
