#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include "properties.h"
extern "C" {
  #include "user_interface.h"
}
// Pin definitions
const int LED_PIN = 5;
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
// Global variables
WiFiClient client;
void turnOff(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);
}
void setup() {
  // disable all output to save power
  turnOff(0);
  turnOff(2);
  turnOff(4);
  turnOff(5);
  //turnOff(12);
  //turnOff(13);
  //turnOff(14);
  turnOff(15);
  // Set up serial console to read web page
  Serial.begin(115200);
  Serial.println("Environment monitor");
  
  // Set up LED for debugging
  //pinMode(LED_PIN, OUTPUT);

  dht.begin();
  
  Wire.begin(SDA_PIN,SCL_PIN);
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
  delay (5000);
  // If there are incoming bytes, print them
  while ( client.available() ) {
    char c = client.read();
    Serial.print(c);
  }
  
  
  
  bailout: Serial.println("Terminate loop");
  // go to deepsleep for 10 minutes
  delay(5000);
  system_deep_sleep_set_option(0);
  system_deep_sleep(15 * 60 * 1000000);
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

  uint16_t lux = lightMeter.readLightLevel();
  Serial.print(", Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  
  int n = sprintf(data,"&field1=%s&field2=%s&field3=%s&field4=%d",humStr,temStr,hIndexStr,lux);
  return n;
}

