
#ifndef _PROPERTIES_H
#define _PROPERTIES_H
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#define SOURCE_KEY "source"
#define SOURCE_ID_KEY "sourceId"
#define MAC_KEY "macAddress"
#define GPS_KEY "gpsLocation"
#define LONG_KEY "longitude"
#define LAT_KEY "latitude"

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
#define writeAPIKey  ""
#define updateInterval  30 * 1000      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)
#define httpPort 80
#define httpsPort 443

#define aqiAddress "api.waqi.info"
#define aqiToken ""
#define aqiStationId "@8767"

#define snifferAddress "sniffer.axonactive.com"
#define snifferUrl "/api/pollutantvalues"
#define sender ""

#define snifferAddress_clone "iot.axonactive.vn"
#define snifferUrl_clone "/sniffer-mind/api/pollutantvalues"
#define sender_clone "143262"

#define longitude "10.831233"
#define latitude "106.635495"

#define TEMP_SENSOR "DHT22"


#define SEND_INTERVAL 15*60*1000

#define DHTPIN 16     // what pin we're connected to

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
extern WiFiClientSecure secureClient;
extern Environment env;
extern bool wifiStatus;

//int readData(char * data);
//bool updateThingSpeak(const char * data);
const char AQI_LEVEL[7][9] = {"Error","Good","Moderate", "Alert","Bad","VeryBad","Poison"};
#define TEMP_MAX 125
#define TEMP_MIN 0
#define HUM_MAX 100
#define HUM_MIN 0

#define MAX_TRY 50
#endif
