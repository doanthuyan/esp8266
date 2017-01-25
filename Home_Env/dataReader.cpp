#include "dataReader.h"
#include <Wire.h> 
#include "properties.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

bool readData(){
  
  // DISPLAY DATA
  /*char humStr[8];
  char temStr[8];
  char hIndexStr[8];
  /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
  
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  delay(200);
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }
  delay(200);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  env.heatIndex = hic;
  env.temperature = t;
  env.humidity = h;

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");

  /*dtostrf(h, 4, 2, humStr);
  dtostrf(t, 4, 2, temStr);
  dtostrf(hic, 4, 2, hIndexStr);


  Serial.println();
  
  int n = sprintf(data,"&field1=%s&field2=%s&field3=%s",humStr,temStr,hIndexStr);
  return n;
  */
  return true;
}
void formatAAVNData(char * dataStr){
  StaticJsonBuffer<300> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();

  JsonObject& source = root.createNestedObject(SOURCE_KEY);
  source[SOURCE_ID_KEY] = sender;
  String macStr = WiFi.macAddress();
  macStr.replace(":","-");
  source[MAC_KEY] = macStr;
  source[GPS_KEY] = gps;

  JsonArray& data = root.createNestedArray(VALS_KEY);
  
  StaticJsonBuffer<150> buffer1;
  JsonObject& obj1 = buffer1.createObject();
  
  obj1[CODE_KEY] = HUM_KEY;
  obj1[SENSOR_KEY] = TEMP_SENSOR;
  JsonObject& val1 = obj1.createNestedObject(VAL_KEY);
  val1[VAL_KEY] = env.humidity;

  StaticJsonBuffer<150> buffer2;
  JsonObject& obj2 = buffer2.createObject();
  obj2[CODE_KEY] = TEMP_KEY;
  obj2[SENSOR_KEY] = TEMP_SENSOR;
  JsonObject& val2 = obj2.createNestedObject(VAL_KEY);
  val2[VAL_KEY] = env.temperature;
  
  /*Serial.println();
  obj1.prettyPrintTo(Serial);
  Serial.println();
  obj2.prettyPrintTo(Serial);
  */
  
  data.add(obj1);
  data.add(obj2);

  Serial.println();
  String returnStr = "";
  root.prettyPrintTo(Serial);
  root.printTo(dataStr,500);
  //return returnStr;
}
void formatThingspeakData(char * data){
    // DISPLAY DATA
  char humStr[8];
  char temStr[8];
  char aqiStr[8];
  //char pm10Str[8];

  dtostrf(env.humidity, 4, 2,humStr );
  dtostrf(env.temperature, 4, 2,temStr );
  dtostrf(env.aqi, 4, 2, aqiStr);
  //dtostrf(env.novaPm10, 4, 2, pm10Str);


  Serial.println();
  
  int n = sprintf(data,"&field1=%s&field2=%s&field3=%s",humStr,temStr,aqiStr);
  //return n;

}
