#include "wifiUtil.h"
#include "properties.h"
#include <ESP8266WiFi.h>

void pushData(){
  // Attempt to connect to website
  Serial.println("\nAttempt to connect to website");
  char data[50];
  int n = readData(data);//sprintf(data,"%s","&field1=1&field2=3&field3=2");
  //data = "&field1=10";
  if(n <= 0){
    Serial.println("\n*** Error reading sensor ***");
    return;
  }
  if ( !updateThingSpeak(data) ) {
    Serial.println("GET request failed");
  }
  delay (5000);
  // If there are incoming bytes, print them
  while ( client.available() ) {
    char c = client.read();
    Serial.print(c);
  }
}
// Attempt to connect to WiFi
void connectWiFi() {
  Serial.println("connecting WiFi");
  byte led_status = 0;
  
  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);
  
  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Blink LED while we wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    digitalWrite(WIFI_ERR_PIN, led_status);
    led_status ^= 0x01;
    delay(100);
  }
  lcd.setCursor(0,1);
  lcd.print("Wifi connected!");
  // Turn LED off when we are connected
  digitalWrite(WIFI_ERR_PIN, LOW);
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
