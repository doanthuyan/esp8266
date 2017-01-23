#ifndef _WIFIUTIL_H
#define _WIFIUTIL_H
void connectWiFi() ;
bool updateThingSpeak(const char * data);
bool updateAAVN();
void pushData();
void getAqiData();
#endif
