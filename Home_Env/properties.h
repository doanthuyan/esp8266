#ifndef _PROPERTIES_H
#define _PROPERTIES_H


#define thingSpeakAddress "api.thingspeak.com"
#define writeAPIKey  "your API key"
#define updateInterval  30 * 1000      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)
#define serverPort 80

#define WIFI_SSID       "ssid"           // cannot be longer than 32 characters!
#define WIFI_PASS       "password"

#define SEND_INTERVAL 15*60*1000

#define SDA_PIN 14
#define SCL_PIN 12

#define DHTPIN 13     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

int readData(char * data);
bool updateThingSpeak(const char * data);
#endif
