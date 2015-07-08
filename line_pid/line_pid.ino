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
 
// pines para los motores
const int VEL_MOT_IZQ = 5;
const int S1_MOT_IZQ = 2;
const int S2_MOT_IZQ = 4;
const int VEL_MOT_DER = 6;
const int S1_MOT_DER = 7;
const int S2_MOT_DER = 8;

// parametros para el PID

const int kp = 2;
const int ki = 5;
const int kd = 1;
const int velBase = 100;

//temporizacion
long tiempoAnterior = 0;
long tiempoActual;
long ts = 100;        //tiempo de muestreo

int Sensores[5];

double Setpoint, Input, Output;

void setup() {
  // inicializar las variables
  Setpoint = 10;
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  for(int i = 5; i => 0; i--)
  {
    digitalWrite(13, 1);
    delay(500);
    digitalWrite(13, 0);
    Serial.println(i);
  }
  
}

void loop() {
  /*
   * el tiempo de muestreo se define con la variable
   * ts, el codigo para calcular el PID y actuar
   * los motores se realiza cada vez que la diferencia
   * de tiempos sea mayor a ts, esto nos garantiza
   * un tiempo de muestreo constante
   */
  tiempoActual = millis();
  if(tiempoActual - tiempoAnterior > ts)
  {
    // actualizamos el tiempo
    tiempoAnterior = tiempoActual;
    // calculamos la posicion de la linea
    Input = Posicion();
    // calculamos el PID
    Output = calcularPID(Setpoint, Input, kp, ki, kd);
    // actuamos los motores
    Motores(Output);
    
  }

}

void Motores(int output)
{
  motIzq(velBase + output);
  motDer(velBase - output);
}

int Posicion(void)
{
  int salida, valor;
  long sum = 0;
  long avg = 0;
  // lectura de los datos
  for(int i = 0; i <= 5; i++)
  {
    Sensores[i] = analogRead(i);
    delay(1);
  }
  //calculo de la posicion de la linea
  for(int i = 0; i <= 5; i++)
  {
    valor = Sensores[i];
    if(valor > 30)  //eliminar ruido
    {
      sum += valor;
      avg += (long)(valor) * (i*1000);
    }
  }
  salida = (int)(avg / sum);
  return salida; 
}

int calcularPID(int setpoint, int input, int kp, int ki, int kd)
{
  float error, errSum, dErr, lastErr, salida;
  error = setpoint - input;
  errSum += error;  //integral
  dErr = (error - lastErr); //derivativa

  //calcular la salida del controlador PID
  salida = kp * error + ki * errSum + kd * dErr;

  //recordar valores anteriores
  lastErr = error;
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


