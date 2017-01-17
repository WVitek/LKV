// Settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

extern uint8_t ethernetMAC[6];
extern uint8_t ethernetIP[4];

extern uint8_t mqttServerIP[4];
extern uint16_t mqttServerPort;

extern char mqttBuf[];

#define USE_DHT
#define USE_DS18
#define USE_DO
#define USE_DI

#ifdef USE_DHT
#include "wDHT.h"
extern DHT dht[];
extern const int nDHT;
#endif // USE_DHT

#ifdef USE_DS18
extern uint8_t pinsDS18[];
extern const int nPinsDS18;
#endif // USE_DS18

#ifdef USE_DI
extern int8_t pinsDI[];
extern const int nPinsDI;
#endif

#endif

