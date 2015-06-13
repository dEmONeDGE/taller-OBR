const int ENA = 3;
const int IN1 = 4;
const int IN2 = 5;


void setup() {
  // put your setup code here, to run once:
  //declarando los pines como salida
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  //habilito el motor
  digitalWrite(ENA, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  //adelante
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  delay(2000);
  //detenido
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  delay(2000);
  //atras
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  delay(2000);
  
}
