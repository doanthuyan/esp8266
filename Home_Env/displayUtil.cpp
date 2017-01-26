#include "displayUtil.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "properties.h"

void initDisplay(){
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Weather station!");
}

void displayData(){
  Serial.println("Displaying data");
  char humStr[8];
  
  char tStr[8];
  char aqiStr[8];
  dtostrf(env.humidity, 2, 0, humStr);
  dtostrf(env.temperature, 2, 0, tStr);
  dtostrf(env.aqi, 4, 0, aqiStr);
  char line1[16];
  char line2[16];
  int n = sprintf(line1,"%s%cC Hum:%s%%",tStr,(char)223,humStr);
  int aqiIndx = getAqiIndex();
  n = sprintf(line2,"AQI:%s_%s",aqiStr,AQI_LEVEL[aqiIndx]);
  Serial.println(line1);
  Serial.println(line2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}
int getAqiIndex(){
  int index = 0;
  if(env.aqi == 0){
    index = 0;
  }else if(env.aqi<= 50){
    index = 1;
  }else if(env.aqi <= 100){
    index = 2;
  }else if(env.aqi <= 150){
    index = 3;
  }else if(env.aqi <= 200){
    index = 4;
  }else if(env.aqi <= 300){
    index = 5;
  }else{
    index = 6;
  }
  return index;
}

