int numero = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(11, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(11) == 0)
  {
    digitalWrite(13, 1);
    delay(1000);
    digitalWrite(13, 0);
    delay(1000);
  }
  
  else
  {
    digitalWrite(13, 1);
    delay(100);
    digitalWrite(13, 0);
    delay(100);
  }
  
}
