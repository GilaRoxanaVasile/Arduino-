#include <LiquidCrystal_PCF8574.h>
#include "Arduino.h"
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int pid;

//pinul becului
#define pinBec 9

//declarari pt citire temperaturii de la senzor
int sensePin = A0;    //pin senzor temperatura
int sensorInput;      //val citita de la pin
double volt2;
float temp_citita;    //temperatura citita de la senzor

//declarari pentru functia afisare_timp()
//#define ROOM_TEMP 20
int timp_inc=40;      //timp incalzire
int timp_men=10;      //timp mentinere
int timp_rac=40;      //timp racire
//int temp=50;        //temepratura finala la care dorim sa ajungem
int temp_finala=35;   //ca sa nu asteptam mult pana ajungem la 50 grade, testam cu 35 in schimb 
float ROOM_TEMP;      //temperatura citita de la senzor
int  total_seconds;   //timpul de cand am inceput procesul
float  moving_sp;     
int racire=0;

//declarari pentru functia PID_Output_Calculate()
float Kp = 20;
float Ki = 0.01;
float Kd = 0.01;
float suma_erori = 0;
float derivativa, eroare_anterioara;
float timp,timp_anterior,dt;

int PID_Output_Calculate(void)
{
  int TSet = moving_sp;        //  --> temepratura la care dorim sa ajungem 
  int T_Current = ROOM_TEMP;   //  --> temperatura curenta citita de la senzor
  int eroare;                  //  --> diferenta dintre temperatura dorita si temperatura curenta
  int output;               

  eroare = TSet - T_Current;
  
  timp_anterior=timp;
  timp=millis();
  dt=(timp-timp_anterior)/1000;
  
  suma_erori = suma_erori + eroare*dt;
  derivativa = (eroare - eroare_anterioara)/dt;
  
  output =(int)( Kp * eroare + Ki * suma_erori + Kd * derivativa);

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


void afisare_timp(void)
{
  int min=0; int sec=0;
  int remaining=0; 

  ROOM_TEMP=masurareTemp(); 
  lcd.setCursor(0,1);
  lcd.print("P:");
  lcd.print(moving_sp);

  double now=millis();
  int total_seconds=now/1000;

  if(total_seconds<=timp_inc)
  {
    Serial.print("Inc: ");
    Serial.print(total_seconds);

    moving_sp = ROOM_TEMP + (temp_finala-ROOM_TEMP)*total_seconds/timp_inc;
    Serial.print("SP: ");
    Serial.println(moving_sp);
  }
  else if ( total_seconds <= (timp_inc + timp_men))
  {
    Serial.print("Men: ");
    Serial.println(total_seconds);
    moving_sp = temp_finala;
  }
  else if ( total_seconds <= (timp_inc + timp_men + timp_rac))
  {
    Serial.print("Rac: ");
    Serial.println(total_seconds);
    racire=1;
    //moving_sp = ROOM_TEMP + (temp_finala - ROOM_TEMP) - (temp_finala - ROOM_TEMP)*total_seconds/(timp_inc + timp_men + timp_rac);
    moving_sp=ROOM_TEMP;  // --> asta pt ca stingem becul si lasam sa se raceasca singur
  }
  else
  {
    Serial.print("oprit: ");
    Serial.println(total_seconds);
  }
  lcd.print("  t:");
  lcd.print(total_seconds);
  delay(500);
}

float masurareTemp()
{
    sensorInput = analogRead(A0);    
    volt2=(sensorInput*5000.0)/ 1023.0;
    temp_citita=volt2/10;        
    return temp_citita;   
}

void setup() {
    pinMode(pinBec, OUTPUT);
    lcd.begin(16, 2);
    lcd.setBacklight(255);
    lcd.clear();
    Serial.begin(9600);
}

void loop() {    
       afisare_timp();
       lcd.setCursor(1,0);
       lcd.print("Temp:");
       lcd.print(ROOM_TEMP);
       if(racire==0)   pid=PID_Output_Calculate();
          else pid=255; 
       analogWrite(pinBec, 255-pid); 
       if(ROOM_TEMP<temp_finala && racire!=1)   //daca inca nu am ajuns la temperatura finala dorita si nici racirea nu o facem
       {
       int wait=1;
       while(wait!=0)       //asteptam pana becul incalzeste suficient cat sa ajungem la temperatura = moving_sp
       {
          delay(100);
          ROOM_TEMP=masurareTemp(); //masuram temp de la senzor la fiecare 0.1 sec
          if(ROOM_TEMP >= moving_sp) wait=0;  //cand am trecut de temp dorita momentan, trecem la urmatorul set point de temp, pana ajungem la cea finala!
       }
       }
       Serial.print("\n");  
}
