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
#ifdef USE_DHT
#include "wDHT.h"
extern DHT dht[];
extern const int nDHT;

#endif // USE_DHT


#endif
