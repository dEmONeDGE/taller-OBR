void setup() {
  // put your setup code here, to run once:
  pinMode(7, INPUT_PULLUP);  //definicion de mi boton
  pinMode(13, OUTPUT); //definicion del led
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(7) == 0)
  {
    digitalWrite(13,1);
    delay(100);
    digitalWrite(13,0);
    delay(100);
  }
  else 
  {
    digitalWrite(13,1);
    delay(1000);
    digitalWrite(13,0);
    delay(1000);
  }
  
  
}
