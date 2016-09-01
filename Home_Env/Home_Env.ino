#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "properties.h"
#include "wifiUtil.h"
#include "dataReader.h"
#include "displayUtil.h"

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3f,16,2);  // set the LCD address to 0x3f for a 16 chars and 2 line display
// Global variables
WiFiClient client;
Environment env;


void setup() {
  initDisplay();

  // Set up serial console to read web page
  Serial.begin(115200);
  Serial.println("Environment monitor");
  
  // Set up LED for debugging
  pinMode(WIFI_ERR_PIN, OUTPUT);

  dht.begin();
  
  // Connect to WiFi
  connectWiFi();
  
  
}

void loop() {
  
  pushData();
  displayData();
  
  
  bailout:Serial.println("Terminate loop");
  // go to deepsleep for 10 minutes
  delay(15*60*1000);

}







