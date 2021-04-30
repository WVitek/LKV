// MQTT_wrk.h
#ifndef _MQTT_WRK_h
#define _MQTT_WRK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

const uint8_t MQTT_QOS0 = 0 << 1;
const uint8_t MQTT_QOS1 = 1 << 1;
const uint8_t MQTT_QOS2 = 2 << 1;

void MQTT_setup();
bool MQTT_loop();

static char sErrValue[] = "-255";
char* MQTT_topic(const char kind[5], const char code[2], const char descr[]);
void MQTT_publish(char* topic, char* msg);
void MQTT_subscribe(char* topic, uint8_t qos);

typedef bool(*FuncMQTThandler)(char*, uint8_t*, unsigned int);

FuncMQTThandler MQTT_setHandler(FuncMQTThandler newCallback);

#endif

