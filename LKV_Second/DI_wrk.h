// DI_wrk.h

#ifndef _DI_WRK_h
#define _DI_WRK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void DI_setup();
bool DI_loop();

#endif

