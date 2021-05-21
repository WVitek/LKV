// settings for LKV-03 (2 этаж, Arduino Uno)

#include "Settings.h"

uint8_t ethernetMAC[6] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEB };
char mqttBuf[] = "03/kkkkk/cc/description";

// KП-3
// Пины 4,10,11,12,13 занимает Ethernet Shield W5100
// 5,6,9 будем использовать для RGB PWM
// 7,8 - DHT-22
// A0,A1 - DS18B20 2шт. (?)
// A2,A3,A4,A5 - DI
// 2,3 - DO


uint8_t ethernetIP[4] = { 192,168,1,223 };
uint8_t mqttServerIP[4] = { 192,168,1,220 }; // lkv & whome
uint16_t mqttServerPort = 1883u;

#ifdef USE_DHT
#include "wDHT.h"
DHT dht[] = { DHT(7, DHT22), DHT(8, DHT22) };
const int nDHT = sizeof(dht) / sizeof(DHT);
#endif

#ifdef USE_DS18
uint8_t pinsDS18[] = { PIN_A0, PIN_A1 };
const int nPinsDS18 = sizeof(pinsDS18) / sizeof(uint8_t);
#endif

#ifdef USE_DI
int8_t pinsDI[] = { PIN_A2, PIN_A3, PIN_A4, PIN_A5 };
const int nPinsDI = sizeof(pinsDI) / sizeof(int8_t);
#endif


#ifdef USE_AI
uint8_t pinsAI[] = { PIN_A4 };
const int nPinsAI = sizeof(pinsAI) / sizeof(uint8_t);
#endif

#ifdef USE_RGB
#include "RGB_wrk.h"
RGB rgb[] = { RGB(5,6,9,0,0) };
const int nRGB = sizeof(rgb) / sizeof(RGB);
#endif

