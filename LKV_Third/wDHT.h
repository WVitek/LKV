/* DHT library

MIT license
written by Adafruit Industries
*/
#ifndef DHT_H
#define DHT_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// Uncomment to enable printing out nice debug messages.
//#define DHT_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif

// Define types of sensors.
#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21


class DHT {
public:
	DHT(uint8_t pin, uint8_t type);
	void begin(void);
	float readTemperature(bool Renew);
	float readHumidity(bool Renew);
	boolean read(bool Renew);
	uint8_t _pin;
	float prevT, prevH;
	uint32_t lastMs;
private:
	uint8_t data[6];
	uint8_t _type, _bit, _port;
	static uint32_t _maxcycles;
	bool _lastresult;

	uint32_t expectPulse(bool level);
};

class InterruptLock {
public:
	InterruptLock() { noInterrupts(); }
	~InterruptLock() { interrupts(); }

};

#endif
