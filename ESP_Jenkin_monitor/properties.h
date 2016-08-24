#ifndef _PROPERTIES_H
#define _PROPERTIES_H

const uint8_t wsIP[] = {192,168,48,20};

#define updateInterval  30 * 1000      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)
#define serverPort 80

#define WIFI_SSID       "***"           // cannot be longer than 32 characters!
#define WIFI_PASS       "***"

#define SEND_INTERVAL 15*60*1000


//deploy SwissDev
#define jenkinJob1  "LUZ_Deploy_To_Server48.53"
#define RED_PIN1 14
#define GREEN_PIN1 12

//deploy VN Dev
#define jenkinJob2  "Z_SLAVE_Deploy_To_VN_DEV_SERVER"
#define RED_PIN2 13
#define GREEN_PIN2 16

//auto build
#define jenkinJob3  "LUZ_Build_DEV"
#define RED_PIN3 4
#define GREEN_PIN3 5

#define jenkinJob4  "alag_ria"
#define RED_PIN4 5
#define GREEN_PIN4 2

#define jenkinJob5  "alag_ria"
#define RED_PIN5 5
#define GREEN_PIN5 2

#define FAILURE_STATUS "FAILURE"
#define UNSTABLE_STATUS "UNSTABLE"
#define ABORTED_STATUS "ABORTED"
#define SUCCESS_STATUS "SUCCESS"

extern IPAddress serverAddr;
void signalTeam(String buildStat);
bool checkJenkinJob(const char * data);
#endif
