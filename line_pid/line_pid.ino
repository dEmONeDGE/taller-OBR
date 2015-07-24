/*
 * line_pid 
 * programa para el control de un robot seguidor
 * de lineas usando un algoritmo de control
 * tipo PID. Se cuenta con un arreglo de 5 
 * sensores infrarrojos detectores de linea
 * los cuales, mediante la funcion posicion()
 * nos dan la posicion de la linea relativa
 * al arreglo se sensores.
 * El algoritmo PID calcula una salida en base
 * a la posicion de la linea y conduce los motores
 * con un driver l298n el cual posee 3 pines de control
 * 
 * Este programa es beerware, si lo utilizas
 * debes invitarme una cerveza cuando me encuentres
 * 
 * Jose Laruta - julio 2015
 */

//librerias para el modulo rf
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
//---------------------------

// RF
RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio
//direcciones
const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format
//-----------------------------

//estructura para los datos a mandar
struct datos_PID{
  int input;
  int error;
  int output;
};

struct datos_t{
  int vel_izq;
  int vel_der;
};
//datos_PID datos;

// pines para los motores
const int VEL_MOT_IZQ = 5;
const int S1_MOT_IZQ = 4;
const int S2_MOT_IZQ = 2;
const int VEL_MOT_DER = 6;
const int S1_MOT_DER = 7;
const int S2_MOT_DER = 8;

// parametros para el PID

const float kp = 1.5;
const float ki = 0.001;
const float kd = 0.8;
int velBase = 70;

//--variables
float error = 0;
float errSum, dErr, lastErr, salida;
float lastInput, dInput;
float ITerm= 0;
//------------------
//temporizacion
long tiempoAnterior = 0;
long tiempoActual;
long ts = 30;        //tiempo de muestreo

int Sensores[5];
int pines[5] = {0, 1, 2, 3, 6};

bool sensores_bin[5];
//variables de interaccion del PID
double Setpoint, Input, Input_ant, Output;

float velMax = 25500.0;
void setup() {
  // inicializar las variables
  // el setpoint se inicializa en 2000 que corresponde al valor del sensor del
  // centro
  Setpoint = 2000;  
  Serial.begin(57600);
  for(int i = 3; i >= 0; i--)
  {
    //digitalWrite(13, 1);
    delay(1000);
    //digitalWrite(13, 0);
    //delay(500);
    Serial.println(i);
  }
  //inicia el rf (SPI)
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  //-----------------------
}

void loop() {
  /*
   * el tiempo de muestreo se define con la variable
   * ts, el codigo para calcular el PID y actuar
   * los motores se realiza cada vez que la diferencia
   * de tiempos sea mayor a ts, esto nos garantiza
   * un tiempo de muestreo constante
   */
  network.update();                          // Check the network regularly

  tiempoActual = millis();
  if(tiempoActual - tiempoAnterior > ts)
  {
    // actualizamos el tiempo
    tiempoAnterior = tiempoActual;
    // calculamos la posicion de la linea
    Input = Posicion(Input_ant);
    Serial.print("input:");
    Serial.print(Input);
    
    // calculamos el PID
    Output = calcularPID(Setpoint, Input, kp, ki, kd)/ 100;
    Serial.print(" output: ");
    Serial.println(Output);
    // actuamos los motores
    Motores(Output);

    datos_t datos = {error, Output};
          
    //envio por el rf
    RF24NetworkHeader header(/*to node*/ other_node);
    //datos_PID datos = {Input, error, Output};
    bool ok = network.write(header,&datos,sizeof(datos));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
    
    Input_ant = Input;
    //
  }

}

void Motores(int output)
{
  motIzq(velBase + output);
  motDer(velBase - output);
  //Serial.print(velBase + output);
  //Serial.print("-");
  //Serial.println(velBase - output);
}

int Posicion(double anterior)
{
  int posicion, valor;
  int linea;
  long sum = 0;
  long avg = 0;
  // lectura de los datos
  for(int i = 0; i < 5; i++)
  {
    Sensores[i] = analogRead(pines[i]);
    //Serial.print(Sensores[i]);
    //Serial.print(" ");
    if(Sensores[i] < 400) sensores_bin[i] = false;
    else sensores_bin[i] = true;
    delay(1);
  }
  Serial.println(" ");
  //calculo de la posicion de la linea
  for(int i = 0; i < 5; i++)
  {
    valor = Sensores[i];
    if(valor > 50)  //eliminar ruido
    {
      sum += valor;
      avg += (long)(valor) * (i*1000);
    }
  }
  posicion = (int)(avg / sum);
  
  if(posicion == -1)
  {
    if(anterior >= 2500) posicion = 5000;
    if(anterior <= 500) posicion = -1000;
  } 
  else 
  {
    if(sensores_bin[0] && sensores_bin[1] && sensores_bin[2]) // angulo de 90 o menor
      posicion = -1000;
    else 
    {
      if(sensores_bin[0] && sensores_bin[1] && sensores_bin[2]) // angulo de 90 o menor
      posicion = 5000;
    }
  }      
  //Serial.println(salida);
  return posicion; 
}

int calcularPID(int setpoint, int input, float kp, float ki, float kd)
{
  /*
   * Funcion para calcular la salida.
   * segun ley de control PID
   * esta funcion implementa un algoritmo PID
   * modificado para calcular la salida del robot,
   * se implementa un PID paralelo con rechazo a 
   * la patada derivativa y anti-windup del integrador
   * 
   */
  error = setpoint - input;
  //control proporcional para la velocidad base
  Serial.print("error: ");
  Serial.println(error);
  if(error < 100) velBase = 75;
  if(error <= 9) velBase = 80;
  else velBase = 65  ;
  ITerm += (ki * error);
  //Serial.print("Iterm: ");
  //Serial.println(ITerm);
  if(ITerm > velMax) ITerm = velMax;
  else if(ITerm < -velMax) ITerm = -velMax;
  
  //errSum += error;  //integral
  
  //dErr = (error - lastErr); //derivativa
  dInput = (Input - lastInput); // patada derivativa
  //Serial.print("dInput");
  //Serial.println(dInput);
  //calcular la salida del controlador PID
  salida = kp * error + ITerm + kd * dInput;
  //if(salida > velMax) salida = velMax;
  //else if(salida < -velMax) salida = -velMax;

  //recordar valores anteriores
  //lastErr = error;
  lastInput = Input;

  //actualizar valores para enviar
  
  return (int)(salida);
}

void Detener()
{
  izqStop();
  derStop();
}

void Conducir(int vel)
{
  if (vel > 0)
  {
    izqAd((byte)(abs(vel)));
    derAd((byte)(abs(vel)));
  }
  else
  {
    izqAt((byte)(abs(vel)));
    derAt((byte)(abs(vel)));
  }
}

void motIzq(int vel)
{
  if (vel > 0)
  {
    izqAd((byte)(abs(vel)));
  }
  else
  {
    izqAt((byte)(abs(vel)));
  }
}

void motDer(int vel)
{
  if (vel > 0)
  {
    derAd((byte)(abs(vel)));
  }
  else
  {
    derAt((byte)(abs(vel)));
  }
}

void izqAd(byte vel)
{
  digitalWrite(S1_MOT_IZQ, 1);
  digitalWrite(S2_MOT_IZQ, 0);
  analogWrite(VEL_MOT_IZQ, vel);
}
void izqAt(byte vel)
{
  digitalWrite(S1_MOT_IZQ, 0);
  digitalWrite(S2_MOT_IZQ, 1);
  analogWrite(VEL_MOT_IZQ, vel);
}
void derAd(byte vel)
{
  digitalWrite(S1_MOT_DER, 1);
  digitalWrite(S2_MOT_DER, 0);
  analogWrite(VEL_MOT_DER, vel);
}
void derAt(byte vel)
{
  digitalWrite(S1_MOT_DER, 1);
  digitalWrite(S2_MOT_DER, 0);
  analogWrite(VEL_MOT_DER, vel);
}

void derStop()
{
  digitalWrite(S1_MOT_DER, 1);
  digitalWrite(S2_MOT_DER, 1);
}
void izqStop()
{
  digitalWrite(S1_MOT_IZQ, 1);
  digitalWrite(S2_MOT_IZQ, 1);
  
}


