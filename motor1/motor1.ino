const int ENA = 5;
const int IN1 = 2;
const int IN2 = 4;


void setup() {
  // put your setup code here, to run once:
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(ENA, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  //girar a la izq IN1 = 0, IN2 = 1
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  delay(2000);
  //girar a la derecha IN1 = 1, IN2 = 0
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  delay(2000);
  
}
