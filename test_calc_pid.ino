

#define pinBec 9
float Kp = 20;
float Ki = 0.01;
float Kd = 0.01;
int suma_erori = 0;
int dt=1;
int derivativa;
int eroare_anterioara;

int PID_Output_Calculate(void)
{
  int TSet = 50;        //  --> temepratura la care dorim sa ajungem 
  int T_Current = 40;   //  --> temperatura curenta citita de la senzor
  int eroare;           //  --> diferenta dintre temperatura dorita si temperatura curenta
  int output;           //  --> pmw?

  eroare = TSet - T_Current;

  suma_erori = suma_erori + eroare*dt;
  derivativa = (eroare - eroare_anterioara)/dt;
  
  output = Kp * eroare + Ki * suma_erori + Kd * derivativa;

  eroare = eroare_anterioara;
  
  if(output > 255)  
  {
    output = 255;
  }
  else if(output < 0)
  {
    output = 0;
  }
  Serial.print(output);
  Serial.print("\n");
  return output;
}

void setup() {
  Serial.begin(9600);
  pinMode(pinBec, OUTPUT);

}

void loop() {
//   uptime=millis()/1000;
 
    analogWrite(pinBec, PID_Output_Calculate());
  
    delay(1000);

}
