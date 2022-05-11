int prim=millis();

void setup() {
  // put your setup code here, to run once:
    pinMode(A0, INPUT);
    pinMode(3, INPUT);
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

    int timpC=millis();
    if(timpC-prim>=1000)
    {
    double temp=analogRead(A0);
    Serial.println("3, ");
    Serial.print(temp);

    double inundatie=digitalRead(3);
    Serial.println("4, ");
    Serial.print(inundatie);
    
    prim=timpC;
    }
}
