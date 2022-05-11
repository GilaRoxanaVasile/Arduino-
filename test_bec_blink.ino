#define control 9

void setup()
{
  pinMode(control, OUTPUT);
  
}

void loop()
{
  digitalWrite(control,HIGH);
  delay(2000);
  digitalWrite(control,LOW);
  delay(2000);
  analogWrite(control, 150);
  delay(2000);
}
