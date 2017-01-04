// MQTT_wrk.h
#ifndef _MQTT_WRK_h
#define _MQTT_WRK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void MQTT_setup();
bool MQTT_loop();

const char sError[] = "error";
char* MQTT_topic(const char kind[5], const char code[2], const char descr[]);
void MQTT_publish(char* topic, char* msg);

#endif

