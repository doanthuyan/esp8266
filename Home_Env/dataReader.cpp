#include "dataReader.h"
#include <Wire.h> 
#include "properties.h"


int readData(char * data){
  
  // DISPLAY DATA
  char humStr[8];
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
    return 0;
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

  dtostrf(h, 4, 2, humStr);
  dtostrf(t, 4, 2, temStr);
  dtostrf(hic, 4, 2, hIndexStr);


  Serial.println();
  
  int n = sprintf(data,"&field1=%s&field2=%s&field3=%s",humStr,temStr,hIndexStr);
  return n;
}
