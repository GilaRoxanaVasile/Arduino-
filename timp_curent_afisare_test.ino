#include <LiquidCrystal_PCF8574.h>
#include "Arduino.h"
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned long startMillis;  
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds (1 second)
unsigned int seconds=0,minutes=0,hours=0;

void setup()
{
 lcd.begin(16, 2);
 lcd.setBacklight(255);
 lcd.clear();
 startMillis = millis();  //initial start time
}

void timp_start()
{
  lcd.setCursor(0,0);
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    seconds++;
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
  if(seconds==60)
  {
    seconds=0; minutes++; lcd.clear();
  }
  if(minutes==60)
  {
    minutes=0; hours++;
  }
  lcd.print(hours);
  lcd.write(':');
  lcd.print(minutes);
  lcd.write(':');
  lcd.print(seconds);
}

void loop()
{
  timp_start();

}
