#include "Common.h"

#ifdef USE_DI

#include "MQTT_wrk.h"
#include "DI_wrk.h"

static uint8_t *stateBits;
const uint8_t prevStateBit = 0x08;
const uint8_t maskReadCount = 0x03;
static uint32_t prevMs;

void DI_setup()
{
	stateBits = (uint8_t*)malloc(nPinsDI * sizeof(uint8_t));
	for (int i = 0; i < nPinsDI; i++)
	{
		int8_t pin = pinsDI[i];
		if (pin < 0)
		{
			pinMode(-pin, INPUT_PULLUP);
			pin = -pin;
		}
		else 
			pinMode(pin, INPUT);
		// special inverted state to publish initial value
		uint8_t prevStateInv = (digitalRead(pin) == HIGH) ? 0: prevStateBit;
		stateBits[i] = prevStateInv | maskReadCount;
	}
}

bool DI_loop()
{
	if (prevMs - millis() < 20)
		return false;
	prevMs = millis();
	bool changed = false;
	for (int i = 0; i < nPinsDI; i++)
	{
		int8_t pin = pinsDI[i];
		if (pin < 0) pin = -pin;
		bool readed = digitalRead(pin) == HIGH;
		uint8_t bits = stateBits[i];
		bool prevState = (bits & prevStateBit) != 0;
		if (readed != prevState)
		{
			uint8_t nReadCount = bits & maskReadCount;
			if (nReadCount > 1)
			{
				char* chan = pin_to_dec(pin);
				static char msg0[] = "0";
				static char msg1[] = "1";
				MQTT_publish(MQTT_topic("DINPS", chan, NULL), readed ? msg1 : msg0);
				changed = true;
				stateBits[i] = readed ? prevStateBit : 0;
			}
			else stateBits[i] = bits & ~maskReadCount | (nReadCount + 1);
		}
		else stateBits[i] = bits & ~maskReadCount; // zero readCoount
	}
	return changed;
}

#endif