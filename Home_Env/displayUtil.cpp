#include "displayUtil.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "properties.h"

void initDisplay(){
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Weather station!");
}

void displayData(){
  char humStr[8];
  char hIndexStr[8];
  dtostrf(env.humidity, 3, 0, humStr);
  dtostrf(env.heatIndex, 2, 0, hIndexStr);

  char line1[16];
  int n = sprintf(line1,"Temp:%s Hum:%s%%",hIndexStr,humStr);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  
}

