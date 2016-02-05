#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <dht.h>
#include <Wire.h>
#include <BH1750.h>
#include "properties.h"

// Pin definitions
const int LED_PIN = 5;
dht DHT;
BH1750 lightMeter;
// Global variables
WiFiClient client;

void setup() {
  
  // Set up serial console to read web page
  Serial.begin(115200);
  Serial.print("Environment monitor");
  
  // Set up LED for debugging
  pinMode(LED_PIN, OUTPUT);
  Wire.begin(14,13);
  lightMeter.begin();
  // Connect to WiFi
  connectWiFi();
  
  
}

void loop() {
  // Attempt to connect to website
  Serial.println("\nAttempt to connect to website");
  char data[50];
  int n = readData(data);//sprintf(data,"%s","&field1=1&field2=3&field3=2");
  //data = "&field1=10";
  if(n <= 0){
    Serial.println("\n*** Error reading sensor ***");
    goto bailout;
  }
  if ( !updateThingSpeak(data) ) {
    Serial.println("GET request failed");
  }
  delay (50000);
  // If there are incoming bytes, print them
  while ( client.available() ) {
    char c = client.read();
    Serial.print(c);
  }
  
  // If the server has disconnected, stop the client and WiFi
  if ( !client.connected() ) {
    Serial.println();
    
    // Close socket and wait for disconnect from WiFi
    client.stop();
    /*if ( WiFi.status() != WL_DISCONNECTED ) {
      WiFi.disconnect();
    }*/
    
    // Turn off LED
    digitalWrite(LED_PIN, LOW);
    
    // Do nothing
    Serial.println("Finished Thing GET test");
    delay (5000);
  }
  delay (SEND_INTERVAL);
  bailout: Serial.println("Terminate loop");
}

// Attempt to connect to WiFi
void connectWiFi() {
  
  byte led_status = 0;
  
  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);
  
  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Blink LED while we wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    digitalWrite(LED_PIN, led_status);
    led_status ^= 0x01;
    delay(100);
  }
  
  // Turn LED on when we are connected
  digitalWrite(LED_PIN, HIGH);
}


bool updateThingSpeak(const char * data)
{
  // Attempt to make a connection to the remote server
  Serial.println("\nAttempt to make a connection to the remote server");
  if ( !client.connect(thingSpeakAddress, serverPort) ) {
    return false;
  }
  
  // Make an HTTP GET request
  client.print("GET /update?key=");
  client.print(writeAPIKey);
  client.print(data);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(thingSpeakAddress);
  client.println("Connection: close");
  client.println();

  return true;

}
int readData(char * data){
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("\nDHT11 status: ");
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error\t"); 
    break;
    default: 
    Serial.print("Unknown error\t"); 
    break;
  }
  // DISPLAY DATA
  char humStr[8];
  char temStr[8];
  /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
  dtostrf(DHT.humidity, 4, 2, humStr);
  dtostrf(DHT.temperature, 4, 2, temStr);
  
  Serial.print("\nHum: ");
  Serial.print(humStr);
  Serial.print(", Tem: ");
  Serial.print(temStr);

  uint16_t lux = lightMeter.readLightLevel();
  Serial.print(", Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  
  int n = sprintf(data,"&field1=%s&field2=%s&field3=%d",humStr,temStr,lux);
  return n;
}

