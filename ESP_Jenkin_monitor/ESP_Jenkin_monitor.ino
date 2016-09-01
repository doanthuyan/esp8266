#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ArduinoJson.h>
#include "properties.h"


// Global variables
WiFiClient client;
IPAddress serverAddr(wsIP[0],wsIP[1],wsIP[2],wsIP[3]);

String returnStr;
  
void setup() {
  
  // Set up serial console to read web page
  Serial.begin(115200);
  Serial.println("Jenkin monitor");
  
  // Set up LED for debugging
  pinMode(RED_PIN1, OUTPUT);
  pinMode(GREEN_PIN1, OUTPUT);
  digitalWrite(RED_PIN1, LOW);
  digitalWrite(GREEN_PIN1, LOW);

  pinMode(RED_PIN2, OUTPUT);
  pinMode(GREEN_PIN2, OUTPUT);
  digitalWrite(RED_PIN2, LOW);
  digitalWrite(GREEN_PIN2, LOW);

  pinMode(RED_PIN3, OUTPUT);
  pinMode(GREEN_PIN3, OUTPUT);
  digitalWrite(RED_PIN3, LOW);
  digitalWrite(GREEN_PIN3, LOW);
  // Connect to WiFi
  connectWiFi();
  
  
}

void loop() {
  // Attempt to connect to website
  int n = 0;
  
  //char returnStr[200];
  Serial.println("\nAttempt to connect to website");
  // Attempt to make a connection to the remote server
  Serial.println("\nAttempt to make a connection to the remote server");
  
  if ( !checkJenkinJob(jenkinJob1,RED_PIN1,GREEN_PIN1) ) {
    Serial.print("GET request failed: ");
    Serial.println(jenkinJob1);
  }
  delay(10000);
  if ( !checkJenkinJob(jenkinJob2,RED_PIN2,GREEN_PIN2) ) {
    Serial.print("GET request failed: ");
    Serial.println(jenkinJob2);
  }
  delay(10000);
  if ( !checkJenkinJob(jenkinJob3,RED_PIN3,GREEN_PIN3) ) {
    Serial.print("GET request failed: ");
    Serial.println(jenkinJob3);
  }
  // If the server has disconnected, stop the client and WiFi
  if ( !client.connected() ) {
    Serial.println();
    
    // Close socket and wait for disconnect from WiFi
    client.stop();
    /*if ( WiFi.status() != WL_DISCONNECTED ) {
      WiFi.disconnect();
    }*/
    
    
    // Do nothing
    Serial.println("Finished Thing GET test");
    delay (5000);
  }
  
  Serial.println("End loop");
  delay (SEND_INTERVAL);
}
void signalTeam(String buildStat,int redPin, int greenPin){

  if(buildStat.equalsIgnoreCase(SUCCESS_STATUS) ){
      digitalWrite(redPin, LOW);
      //digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(greenPin, HIGH);
  }else if(buildStat.equalsIgnoreCase(FAILURE_STATUS) ){
      digitalWrite(redPin, HIGH);
      //digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(greenPin, LOW);
  }/*else if(buildStat.equalsIgnoreCase(UNSTABLE_STATUS) ){
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
  }*/else{
      digitalWrite(redPin, LOW);
      //digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(greenPin, LOW);
  }


}
// Attempt to connect to WiFi
void connectWiFi() {
  Serial.println("connectWiFi");
  byte led_status = 0;
  
  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);
  
  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Blink LED while we wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    //digitalWrite(LED_PIN, led_status);
    //led_status ^= 0x01;
    delay(100);
  }
  
}


bool checkJenkinJob(const char * jobName,int redPin, int greenPin)
{
  if ( !client.connect(serverAddr, serverPort) ) {
    Serial.println("\nConnect Failed");
    return false;
  }
  Serial.print("\n\nRequest Job status:" );
  Serial.println(jobName);
  String responseStr = "";
  char ipStr[20];
  int n=sprintf (ipStr,"%u.%u.%u.%u:%u" ,wsIP[0],wsIP[1],wsIP[2],wsIP[3],serverPort );

  // Make an HTTP GET request
  client.print("GET /jenkins/job/");
  client.print(jobName);
  client.print("/lastBuild/api/json?tree=result,building");
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(ipStr);
  client.println("Connection: close");
  client.println();

  delay (10000);
  // If there are incoming bytes, print them
  while ( client.available() ) {
    char c = client.read();
    
    responseStr.concat(c);
    Serial.print(c);
    n++;
  }
  //Get response data
  int blnkLineIndex = responseStr.lastIndexOf('\n',responseStr.length()-2) ;
  String data = responseStr.substring(blnkLineIndex);
  Serial.println("\nResponse Data: " + data);
 
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data.c_str());
  Serial.print("\n\nReceived data: \n");
  root.prettyPrintTo(Serial);
  signalTeam(root["result"],redPin,greenPin);
  return true;

}


