#include <EEPROM.h>
#include <time.h>

int prim=millis();
time_t rawtime=1651598055;


//citire temperatura senzor LM35
int tempPin = A0; //pin senzor temperatura
int sensorInput; //val citita de la pin
double volt2;
float temp_citita; //temperatura citita de la senzor

int inundatiePin=3;
int secunde=0;
int minute=10; 
int ora=12;
char timp[10];
char moment[20];
//char mom_mes[10];

typedef struct sMesaje{
  bool stare;
  char timp[20];
  char mesaj[32];
}message;

message msg[10];

float masurareTemp()
{
    sensorInput = analogRead(A0);
    volt2=(sensorInput*5000.0)/ 1023.0;
    temp_citita=volt2/10;
    return temp_citita;
}

char mesaje[10][53];
char msg2[32]="";
char mesaj[32]="";
char mesajFinal[32]="";
char inundatii[10][20];

void clearEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) 
  {
    EEPROM.write(i, 0);
  }
}

void cautLoc(){

  int minim=0;
  
  for(int i=1;i<10;i++)
  {
    if(strcmp(msg[minim].timp,msg[i].timp)>0)
          minim=i;
  }
  msg[minim].stare=false;
  strcpy(msg[minim].timp,moment);
  strcpy(msg[minim].mesaj,mesaj);
  
}

void setup() {

    pinMode(tempPin, INPUT);
    pinMode(inundatiePin, INPUT);
    
    secunde= prim/1000;
    Serial.begin(9600);

    //clearEEPROM();
    
    EEPROM.get(0,msg);
    Serial.println("---MESAJE---");
    for(int i=0;i<10;i++)
    {
      Serial.print(i);
      Serial.print(". ");
      Serial.print(msg[i].stare);
      Serial.print(" ");
      Serial.print(msg[i].timp);
      Serial.print(" ");
      Serial.println(msg[i].mesaj);
    }

    EEPROM.get(550,inundatii);
    Serial.println("---INUNDATII---");
    for(int i=0;i<10;i++)
    {
      Serial.print(i);
      Serial.print(". ");
      Serial.println(inundatii[i]);
    }
}

void moment_curent()
{
  time_t     now;
  struct tm  ts;
  char       buf[80];

//  now=time(0);

  /*  struct tm strtime;
    time_t timeoftheday;
 
    strtime.tm_year = 2022-1900;
    strtime.tm_mon = 5;
    strtime.tm_mday = 3;
    strtime.tm_hour = 9;
    strtime.tm_min = 50;
    strtime.tm_sec = 58;
    strtime.tm_isdst = 0;
 
    timeoftheday = mktime(&strtime);

    Serial.println(ctime(&timeoftheday));
    strcpy(moment, ctime(&timeoftheday));
  
   strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeoftheday);
  */
    rawtime = rawtime + millis()/1000;
   // ts.tm_year=2022-1900;
    ts = *localtime(&rawtime);
    ts.tm_year=2022-1900;
    ts.tm_mday+=1;
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
    Serial.println(buf);
    strcpy(moment, buf);
}

void loop() {
    
    int timpC=millis();
    if(timpC-prim>=1000)
    {
      float temp=masurareTemp();
      Serial.print("3, ");
      Serial.println(temp);
      
      double inundatie=digitalRead(inundatiePin);
      Serial.print("4, ");
      Serial.println(inundatie);
      
      prim=timpC;
      secunde++;
    }
    if(secunde==60)
    {
      minute++;    secunde=0;
    }
    if(minute==60)
    {
      ora++;      minute=0;
    }
    
    if(Serial.available())
    {
      String str=Serial.readString();
      int optiune=str[0]-'0';
      if(optiune==6)
      {
        str.toCharArray(msg2,32);
        memcpy(mesaj, msg2+3, sizeof(msg2)); //copiez in mesaj valoarea lui msg2 fara primele trei caractere ca sa scap de "6, "
        Serial.print("Mesajul primit este: ");
        Serial.println(mesaj);
        moment_curent();
        Serial.print("TIMP: ");
        Serial.println(moment);
        int i;
        for(i=0;i<10;i++)
          if(strlen(msg[i].mesaj)==0)
          {
            msg[i].stare=false;
            strcpy(msg[i].timp, moment);
            strcpy(msg[i].mesaj, mesaj);
            break;
          }
        if(i==10)
          cautLoc();
        EEPROM.put(0, msg);
        memset(mesaj, 0, sizeof(mesaj));
        memset(moment, 0, sizeof(moment));
    }
    if(optiune==5)
    {
      moment_curent();
      Serial.print("Inundatie detectata la momentul: ");
      Serial.println(moment);
      int i;
      for(i=0;i<10;i++)
        if(strlen(inundatii[i])==0)
        {
          strcpy(inundatii[i], moment);
          break;
        }
      if(i==10)
        strcpy(inundatii[0], moment);
      EEPROM.put(550,inundatii);
      memset(moment, 0, sizeof(moment));
    }
    }

}
