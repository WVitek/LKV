// RGB_wrk.h

#ifdef USE_RGB

#ifndef _RGB_WRK_h
#define _RGB_WRK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class RGB {
public:
	RGB(uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t pinW1, uint8_t pinW2);
	void set(char hexRGBWW[]);
	void setup();
private:
	uint8_t pins[5];
};

void RGB_setup();

#endif

#endif

