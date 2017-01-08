// Common.h

#ifndef _COMMON_h
#define _COMMON_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Settings.h"

void common_setup();

void common_loop();

char* pin_to_dec(int pin);
int dec_to_pin(char *twoCharStr);

#endif

