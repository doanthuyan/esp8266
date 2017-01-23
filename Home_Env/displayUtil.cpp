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
  
  char tStr[8];
  char aqiStr[8];
  dtostrf(env.humidity, 3, 0, humStr);
  dtostrf(env.temperature, 2, 0, tStr);
  dtostrf(env.aqi, 3, 0, aqiStr);
  char line1[16];
  char line2[16];
  int n = sprintf(line1,"Temp:%s Hum:%s%%",tStr,humStr);
  int aqiIndx = getAqiIndex();
  n = sprintf(line1,"AQI:%s %s",aqiStr,AQI_LEVEL[aqiIndx]);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  
}
int getAqiIndex(){
  int index = 0;
  if(env.aqi<= 50){
    index = 0;
  }else if(env.aqi <= 100){
    index = 1;
  }else if(env.aqi <= 150){
    index = 2;
  }else if(env.aqi <= 200){
    index = 3;
  }else if(env.aqi <= 300){
    index = 4;
  }else{
    index = 5;
  }
  return index;
}

