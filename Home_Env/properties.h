#ifndef _PROPERTIES_H
#define _PROPERTIES_H
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <WiFiClient.h>

#define SOURCE_KEY "source"
#define SOURCE_ID_KEY "sourceId"
#define MAC_KEY "macAddress"
#define GPS_KEY "gpsLocation"

#define VALS_KEY "values"
#define VAL_KEY "value"
#define MEASURE_KEY "measuredAt"

#define CODE_KEY "code"
#define SENSOR_KEY "sensor"

#define O3_KEY "O3"
#define PM25_KEY "PM2.5"
#define PM10_KEY "PM10"
#define HUM_KEY "HUM"
#define TEMP_KEY "TEMP"


#define thingSpeakAddress "api.thingspeak.com"
#define writeAPIKey  "7U61IICUDWDO0TGV"
#define updateInterval  30 * 1000      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)
#define serverPort 80

#define aqiAddress "api.waqi.info"
#define aqiToken "d529395274dba5229ec9b822098f340a658b2a75"
#define aqiStationId "@8767"

#define snifferAddress "iot.axonactive.vn"
#define snifferUrl "/sniffer-mind/api/pollutantvalues"
#define sender "oLHLDtpBxS"

#define gps "10.831233, 106.635495"

#define TEMP_SENSOR "DHT11"


#define WIFI_SSID       "IoT"           // cannot be longer than 32 characters!
#define WIFI_PASS       "IoT@AAVN"

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
  float aqi;
};

extern DHT dht;
extern LiquidCrystal_I2C lcd;
extern WiFiClient client;
extern Environment env;

int readData(char * data);
bool updateThingSpeak(const char * data);
const char AQI_LEVEL[6][9] = {"Good","Moderate", "Alert","Bad","VeryBad","Poison"};

#endif
