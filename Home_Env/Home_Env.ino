#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "properties.h"
#include "wifiUtil.h"
#include "dataReader.h"
#include "displayUtil.h"
#include <ArduinoJson.h>

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3f for a 16 chars and 2 line display
// Global variables

Environment env;


void setup() {
  
  

  // Set up serial console to read web page
  Serial.begin(115200);
  Serial.println("Environment monitor");
  initDisplay();
  // Set up LED for debugging
  pinMode(WIFI_ERR_PIN, OUTPUT);

  dht.begin();
  
  // Connect to WiFi
  connectWiFi();
  
  
}

void loop() {
  getAqiData();
  pushData();
  displayData();

  delay (500);
  
  //bailout:Serial.println("Terminate loop");
  // go to deepsleep for 10 minutes
  delay(15*60*1000);
  
}







