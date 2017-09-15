#include "wifiUtil.h"
#include "properties.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "dataReader.h"


void pushData(){
  // Attempt to connect to website
  Serial.println("\nAttempt to connect to website");
  //data = "&field1=10";

  
  
  if ( !updateThingSpeak() ) {
    Serial.println("GET request failed");
  }
  
  if ( !updateAAVN() ) {
    Serial.println("POST sniffer request failed");
  }
  /*if ( !updateAAVN_clone() ) {
    Serial.println("POST sniffer-mind request failed");
  }*/
}
// Attempt to connect to WiFi
void connectWiFi() {
  Serial.println("\nconnecting WiFi");
  
  byte led_status = 0;
  
  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);
  
  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Blink LED while we wait for WiFi connection
  int count = 0;
  while ( WiFi.status() != WL_CONNECTED && count < MAX_TRY) {
    digitalWrite(WIFI_ERR_PIN, led_status);
    led_status ^= 0x01;
    delay(500);
    count ++;
  }
  if(WiFi.status() == WL_CONNECTED){
    wifiStatus = true;
    Serial.println("Wifi connected");
  }else{
    wifiStatus = false;
    Serial.println("Wifi fail");
  }
  //lcd.setCursor(0,1);
  //lcd.print("Wifi connected!");
  // Turn LED off when we are connected
  digitalWrite(WIFI_ERR_PIN, LOW);
}

bool updateThingSpeak()
{
  WiFiClient client;
  char data[100];
  //int n = readData(data);//sprintf(data,"%s","&field1=1&field2=3&field3=2");
  formatThingspeakData(data);

  // Attempt to make a connection to the remote server
  Serial.println("\nAttempt to make a connection to the remote server");
  if ( !client.connect(thingSpeakAddress, httpPort) ) {
    return false;
  }
  
  // Make an HTTP GET request
  //debug
  Serial.print("GET /update?key=");
  Serial.print(writeAPIKey);
  Serial.print(data);
  Serial.println(" HTTP/1.1");
  Serial.print("Host: ");
  Serial.println(thingSpeakAddress);
  Serial.println("Connection: close");
  Serial.println();
  //
  client.print("GET /update?key=");
  client.print(writeAPIKey);
  client.print(data);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(thingSpeakAddress);
  client.println("Connection: close");
  client.println();
  Serial.println("\n\n---------------------------------------------------------------------\n");
  Serial.println("RESponse: \n" );

  delay (5000);
  // If there are incoming bytes, print them
  while ( client.available() ) {
    char c = client.read();
    Serial.print(c);
  }
  Serial.println("\n---------------------------------------------------------------------\n");
  client.stop();
  return true;

}
void getAqiData(){
  WiFiClient client;
  Serial.println("\nAttempt to connect to website");
  String response  = "";
  Serial.println("\nAttempt to read data from AQICN");
  if ( !client.connect(aqiAddress, httpPort) ) {
    Serial.println("Cannot connect AQI data");
    return;
  }
  
  // Make an HTTP GET request
  client.print("GET /feed/");
  client.print(aqiStationId);
  client.print("/?token=");
  client.print(aqiToken);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(aqiAddress);
  client.println("Connection: close");
  client.println();

  

  delay (5000);
  // If there are incoming bytes, print them
  //String response = "";
  while ( client.available() ) {
    char c = client.read();
    response.concat(c);
    Serial.print(c);
  }
  client.stop();
  Serial.println();
  int index = response.indexOf('{');
  response = response.substring(index);
  response.replace ("\\","");
  Serial.println("Received data:" );
  Serial.println(response);
  char dataStr[1000];
  strcpy(dataStr, response.c_str());
  StaticJsonBuffer<1200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(dataStr);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  JsonObject& obj1 = root["data"];
  env.aqi = obj1["aqi"];
  //Serial.println(data);

  Serial.print("Received data:" );
  Serial.println(response);
  Serial.print("AQI: ");
  Serial.println(env.aqi);

}
bool updateAAVN()
{
  WiFiClientSecure secureClient;
  //WiFiClient client;
  // Attempt to make a connection to the remote server
  Serial.println("\nAttempt to make a connection to the remote server");
  if ( !secureClient.connect(snifferAddress, httpsPort) ) {
    Serial.println("connection failed");
    return false;
  }
  Serial.println("\n---------------------------------------------------------------------\n");
  Serial.println("REQUEST: \n" );
  // We now create a URI for the request
  //String url = "/sniffer-admin/api/packet";
  //String postData = "{\"source\": {\"senderCode\": \"143253\",\"netAddress\": \"192.168.1.1\"},\"data\": [{\"symbolCode\": \"O3\",\"value\": 4.1}, {\"symbolCode\": \"PM2.5\",\"value\": 55}]}";
  char data[500];
  formatAAVNData(data,sender);
  Serial.print("Data: ");
  Serial.println(data);
  
  Serial.print("\n\nRequesting URL: ");
  Serial.println(snifferUrl);
  Serial.println("\n---------------------------------------------------------------------\n");
  String len = String(strlen(data));
  String requestStr = String("POST ") + snifferUrl + " HTTP/1.1\r\n" +
               "Host: " + snifferAddress + "\r\n" +
               "User-Agent: SnifferHub\r\n" +
               "Content-Length: " + len +"\r\n"+
               "Content-Type: application/json\r\n"+
               "Connection: close\r\n"+
               "\r\n"+
               data+
               "\r\n";
  Serial.println(requestStr);
  // This will send the request to the server
  secureClient.print(requestStr);
  
  // If there are incoming bytes, print them
  Serial.println("\n\n---------------------------------------------------------------------\n");
  Serial.println("RESPONSE: \n" );
  // Read all the lines of the reply from server and print them to Serial
  int waitcount = 0;
  while (!secureClient.available() && waitcount++ < 300) {
      Serial.println(".");
       delay(10);
  }
  
  int count = 0;
  while ( secureClient.available() ) {
    char c = secureClient.read();
    Serial.print(c);
    count++;
  }
  if(count >0){
    return true;
  }else{
    return false;
  }

  



}
bool updateAAVN_clone()
{
  WiFiClientSecure secureClient;
  //WiFiClient client;
  // Attempt to make a connection to the remote server
  Serial.println("\nAttempt to make a connection to the remote server");
  if ( !secureClient.connect(snifferAddress_clone, httpsPort) ) {
    Serial.println("connection failed");
    return false;
  }
  Serial.println("\n---------------------------------------------------------------------\n");
  Serial.println("REQUEST: \n" );
  // We now create a URI for the request
  //String url = "/sniffer-admin/api/packet";
  //String postData = "{\"source\": {\"senderCode\": \"143253\",\"netAddress\": \"192.168.1.1\"},\"data\": [{\"symbolCode\": \"O3\",\"value\": 4.1}, {\"symbolCode\": \"PM2.5\",\"value\": 55}]}";
  char data[500];
  formatAAVNData(data,sender_clone);
  Serial.print("Data: ");
  Serial.println(data);
  
  Serial.print("\n\nRequesting URL: ");
  Serial.println(snifferUrl_clone);
  Serial.println("\n---------------------------------------------------------------------\n");
  Serial.print("POST ");
  Serial.print(snifferUrl_clone);
  Serial.println(" HTTP/1.0");
  Serial.print("Host: ");
  Serial.println(snifferAddress_clone);
  Serial.println("Accept: */*");
  Serial.println("Cache-Control: no-cache");
  Serial.println("Content-Type: application/json");
  Serial.print("Content-Length: ");
  Serial.println(strlen(data));
  Serial.println();
  Serial.println(data);
  // This will send the request to the server
  secureClient.print("POST ");
  secureClient.print(snifferUrl_clone);
  secureClient.println(" HTTP/1.0");
  secureClient.print("Host: ");
  secureClient.println(snifferAddress_clone);
  secureClient.println("Accept: */*");
  secureClient.println("Cache-Control: no-cache");
  secureClient.println("Content-Type: application/json");
  secureClient.print("Content-Length: ");
  secureClient.println(strlen(data));
  secureClient.println();
  secureClient.println(data);
  //client.println("Connection: close");
  //client.println();
  delay(100);
   delay (5000);
  // If there are incoming bytes, print them
  Serial.println("\n---------------------------------------------------------------------\n");
  Serial.println("RESponse: \n" );

  while ( secureClient.available() ) {
    char c = secureClient.read();
    Serial.print(c);
  }
  Serial.println("\n---------------------------------------------------------------------\n");
  return true;

}
