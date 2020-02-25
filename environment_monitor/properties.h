#ifndef _PROPERTIES_H
#define _PROPERTIES_H

#include <dht.h>

#define thingSpeakAddress "api.thingspeak.com"
#define writeAPIKey  ""
#define updateInterval  30 * 1000      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)
#define serverPort 80


#define SEND_INTERVAL 15*60*1000

#define DHT11_PIN 12
#define SDA_PIN 14
#define SCL_PIN 13

extern dht DHT;
extern BH1750 lightMeter;

int readData(char * data);
bool updateThingSpeak(const char * data);
#endif
