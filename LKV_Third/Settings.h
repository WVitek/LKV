// Settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define USE_DHT
#define USE_DS18
#define USE_DO
//#define USE_SERVO
#define USE_DI
//#define USE_AI
#define USE_RGB

#ifdef USE_DHT
#include "wDHT.h"
extern DHT dht[];
extern const int nDHT;
#endif

#ifdef USE_DS18
extern uint8_t pinsDS18[];
extern const int nPinsDS18;
#endif

#ifdef USE_DI
extern int8_t pinsDI[];
extern const int nPinsDI;
#endif

#ifdef USE_AI
extern uint8_t pinsAI[];
extern const int nPinsAI;
#endif

#ifdef USE_RGB
#include "RGB_wrk.h"
extern RGB rgb[];
extern const int nRGB;
#endif

#define USE_DEBUG

#ifdef USE_DEBUG
#define DEBUG_PRINT(...) { Serial.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { Serial.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif

#endif

