// DHT_wrk.h

#ifndef _DHT_WRK_h
#define _DHT_WRK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

bool DHT_setup();
bool DHT_loop();

#endif

