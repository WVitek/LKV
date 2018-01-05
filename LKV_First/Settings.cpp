// settings for LKV-01 (Котельная)

#include "Settings.h"

uint8_t ethernetMAC[6] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
char mqttBuf[] = "01/kkkkk/cc/description";

//*
// @home
uint8_t ethernetIP[4] = { 192,168,1,220 };
uint8_t mqttServerIP[4] = { 192,168,1,200 };
uint16_t mqttServerPort = 1883u;
/*/
// @work
uint8_t ethernetIP[4] = { 172,22,130,220 };
//uint8_t ethernetIP[4] = { 192,168,31,220 }; // через роутер
uint8_t mqttServerIP[4] = { 172,22,130,44 };
uint16_t mqttServerPort = 3389u;
//*/

#ifdef USE_DHT
#include "wDHT.h"
DHT dht[] = { DHT(36, DHT11), DHT(38, DHT11) };
const int nDHT = sizeof(dht) / sizeof(DHT);
#endif

#ifdef USE_DS18
uint8_t pinsDS18[] = { 32, 33 };
const int nPinsDS18 = sizeof(pinsDS18) / sizeof(uint8_t);
#endif

#ifdef USE_DI
int8_t pinsDI[] = { -22, -24, -26, 37 };
const int nPinsDI = sizeof(pinsDI) / sizeof(int8_t);
#endif

#ifdef USE_AI
uint8_t pinsAI[] = { 0, 1 };
const int nPinsAI = sizeof(pinsAI) / sizeof(uint8_t);
#endif

