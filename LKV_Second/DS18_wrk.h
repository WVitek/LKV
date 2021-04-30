// DS18_wrk.h

#ifndef _DS18_WRK_h
#define _DS18_WRK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

bool DS18_setup();
bool DS18_loop();

#endif

