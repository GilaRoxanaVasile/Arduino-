#include <EEPROM.h>

int prim=millis();

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

float masurareTemp()
{
    sensorInput = analogRead(A0);
    volt2=(sensorInput*5000.0)/ 1023.0;
    temp_citita=volt2/10;
    return temp_citita;
}

char mesaje[10][32];
char msg2[32]="";
char mesaj[32]="";
char mesajFinal[32]="";


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
    if(strcmp(mesaje[minim],mesaje[i])>0)
          minim=i;
  }

  strcpy(mesaje[minim],mesajFinal);
  
}

void setup() {

    pinMode(tempPin, INPUT);
    pinMode(inundatiePin, INPUT);
    
    char msg[32]="";
    secunde= prim/1000;
    Serial.begin(9600);

   // clearEEPROM();
    EEPROM.get(0,mesaje);
    
    for(int i=0;i<10;i++)
    {
      Serial.print(i);
      Serial.print(". ");
      Serial.println(mesaje[i]);
    }

}

void timpCurent()
{
    timp[0]=ora/10+'0';
    timp[1]=ora%10+'0';
    timp[2]=':';
    timp[3]=minute/10+'0';
    timp[4]=minute%10+'0';
    timp[5]=':';
    timp[6]=secunde/10+'0';
    timp[7]=secunde%10+'0';
    timp[8]=' ';    
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
        timpCurent();
        Serial.print("Ora: ");
        Serial.println(timp);
        memcpy(mesajFinal, timp, 9);
        strcat(mesajFinal, mesaj);
        Serial.println(mesajFinal);
        int i;
        for(i=0;i<10;i++)
          if(strlen(mesaje[i])==0)
          {
            strcpy(mesaje[i], mesajFinal);
            break;
          }
        if(i==10)
          cautLoc();
        EEPROM.put(0, mesaje);
        memset(mesajFinal, 0, sizeof(mesajFinal));
    }
    }

}
