/*
 * bismillah hir rahman nir raheem
 * ESP8266 = Pin D7 & Pin D8
 * Note: Uno and ESP8266 cross connection
 */

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial NodeMCU(D7,D8);
#define CO_KEY "CO"
#define CO2_KEY "CO2"
void setup() {
    Serial.begin(115200);
    NodeMCU.begin(9600);

    pinMode(D7,INPUT);
    pinMode(D8,OUTPUT);
}

void loop() {
    String content = "";
    char character;

    while (NodeMCU.available()) {
       character = NodeMCU.read();
       content.concat(character);
    }

    if (content != "") {
        Serial.println(content);
    }

    float co = 0,co2 = 0;
    int startIndex = content.indexOf(CO_KEY); 
    int endIndex = content.indexOf(",");
    if(startIndex >= 0){
      if(endIndex >0){
        co = content.substring(startIndex+strlen(CO_KEY),endIndex).toFloat();
      }else{
        co = content.substring(startIndex+strlen(CO_KEY)).toFloat();
      }
    }
    startIndex = content.indexOf(CO2_KEY); 
    endIndex = content.indexOf(",");
    if(startIndex >= 0){
      if(endIndex >0){
        co2 = content.substring(startIndex+strlen(CO2_KEY),endIndex).toFloat();
      }else{
        co = content.substring(startIndex+strlen(CO2_KEY)).toFloat();
      }
    }
    Serial.println("CO: ");
    Serial.println(co);
    Serial.println("CO2: ");
    Serial.println(co2);
    delay(2000);
}
