//timer varianta cu millis()
#include <LiquidCrystal_PCF8574.h>
#include "Arduino.h"
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned long startMillis;  
unsigned long currentMillis;
const unsigned long period = 1000;  // 1 second
unsigned int seconds=0,minutes=0,hours=0;

void setup()
{
 lcd.begin(16, 2);
 lcd.setBacklight(255);
 lcd.clear();
 startMillis = millis();  //initial start time
}

void loop()
{
 // lcd.clear();
  lcd.setCursor(0,0);
  currentMillis = millis();  // the number of milliseconds since the program started
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    seconds++;
    startMillis = currentMillis;  
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
