#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
float numar_CAN, tens, t;
void setup()
{
    adc_init ();
  lcd.init();
  lcd.backlight();  
}
void loop()
{
  lcd.setCursor(1,0);
  temperatura();
  delay(500);
}

void adc_init() //adc initialization 
{ 
 ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); //set division factor between system clock 
                                                   //frequency and the input clock to the ADC- 128 
 ADMUX |= (1<<REFS0); //AVcc with external capacitor at Aref pin 
 ADCSRA |= (1<<ADEN);//enable ADC 
 ADCSRA |= (1<<ADSC);//ADC start conversion 
} 

uint16_t read_adc(uint8_t channel) 
{ 
 ADMUX &= 0xF0;//set input AO to A5
 ADMUX |= channel;//select chanel AO to A5
 ADCSRA |= (1<<ADSC); //start conversion
 while(ADCSRA & (1<<ADSC));//wait wile adc conversion are not updated 
 return ADCW; //read and return voltage 
}
void temperatura()
{
  numar_CAN=read_adc(0);
  //tens=(numar_CAN/1024.0)*5000;
  tens=(numar_CAN*5000.0)/1024.0;
  t=(tens)/10;
  lcd.print(t);
}
