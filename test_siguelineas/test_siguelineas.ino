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
  pinMode(EN_M_IZQ, OUTPUT);
  pinMode(S1_M_IZQ, OUTPUT);
  pinMode(S2_M_IZQ, OUTPUT);
  pinMode(EN_M_DER, OUTPUT);
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
    adelante();
    
  }
  else if(izq_negro && !der_negro)
  {
    //izquierda
    izquierda();
  }
  else if(!izq_negro && der_negro)
  {
    //der
    derecha();
  }
  else if(izq_negro && der_negro)
  {
    //detener
    detener();
  }
  else detener();//detener
}

void adelante(void)
{
  //motor Izq
  digitalWrite(EN_M_IZQ, 1);
  digitalWrite(S1_M_IZQ, 1);
  digitalWrite(S2_M_IZQ, 0);
  //motor der
  digitalWrite(EN_M_DER, 1);
  digitalWrite(S1_M_DER, 1);
  digitalWrite(S2_M_DER, 0);
}
void izquierda(void)
{
  //motor Izq
  digitalWrite(EN_M_IZQ, 0);
  digitalWrite(S1_M_IZQ, 1);
  digitalWrite(S2_M_IZQ, 0);
  //motor der
  digitalWrite(EN_M_DER, 1);
  digitalWrite(S1_M_DER, 1);
  digitalWrite(S2_M_DER, 0);
}
void derecha(void)
{
  //motor Izq
  digitalWrite(EN_M_IZQ, 1);
  digitalWrite(S1_M_IZQ, 1);
  digitalWrite(S2_M_IZQ, 0);
  //motor der
  digitalWrite(EN_M_DER, 0);
  digitalWrite(S1_M_DER, 1);
  digitalWrite(S2_M_DER, 0);
}
void detener(void)
{
  //motor Izq
  digitalWrite(EN_M_IZQ, 0);
  
  //motor der
  digitalWrite(EN_M_DER, 0);
  
}
