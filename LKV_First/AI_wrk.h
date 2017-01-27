// AI_wrk.h

#ifndef _AI_WRK_h
#define _AI_WRK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void AI_setup();
bool AI_loop();

#endif

