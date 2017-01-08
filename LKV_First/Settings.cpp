// settings for LKV-01 (Котельная)

#include "Settings.h"

uint8_t ethernetMAC[6] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
uint8_t ethernetIP[4] = { 192,168,1,220 };
char mqttBuf[] = "01/kkkkk/cc/description";

uint8_t mqttServerIP[4] = { 192,168,1,200 };
uint16_t mqttServerPort = 1883u;

#ifdef USE_DHT
DHT dht[] = { DHT(7, DHT22), DHT(32, DHT22) };
const int nDHT = sizeof(dht) / sizeof(DHT);
#endif

#ifdef USE_DS18
uint8_t pinsDS18[] = { 6 };
const int nPinsDS18 = sizeof(pinsDS18) / sizeof(uint8_t);
#endif

#ifdef USE_DI
int8_t pinsDI[] = { 40,41,42,43,44,45,46,47,48,49,50,51,52,53 };
const int nPinsDI = sizeof(pinsDI) / sizeof(int8_t);
#endif