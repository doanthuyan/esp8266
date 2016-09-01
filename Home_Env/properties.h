#ifndef _PROPERTIES_H
#define _PROPERTIES_H
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <WiFiClient.h>

#define thingSpeakAddress "api.thingspeak.com"
#define writeAPIKey  "7U61IICUDWDO0TGV"
#define updateInterval  30 * 1000      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)
#define serverPort 80

#define WIFI_SSID       "anonymous"           // cannot be longer than 32 characters!
#define WIFI_PASS       "anhtu4x4"

#define SEND_INTERVAL 15*60*1000

#define DHTPIN 13     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define WIFI_ERR_PIN 12

struct Environment{
  float humidity;
  float temperature;
  float heatIndex;
  
};

extern DHT dht;
extern LiquidCrystal_I2C lcd;
extern WiFiClient client;
extern Environment env;

int readData(char * data);
bool updateThingSpeak(const char * data);
#endif
