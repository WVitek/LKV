#include "Common.h"

#ifdef USE_DS18

#include "MQTT_wrk.h"
#include <DallasTemperature.h>
#include <OneWire.h>

const uint16_t POLL_PERIOD = 30000u / nDS18;
const uint32_t REFRESH_PERIOD = 300000ul;
static DallasTemperature ds;

class PollData
{
public:
	PollData(uint8_t pin);
	OneWire wire;
	DeviceAddress addr;
	uint32_t lastMs;
	float prevT;
private:
};

PollData::PollData(uint8_t pin)
	:wire(pin)
{
	ds.setOneWire(&wire);
	ds.getAddress(&(addr[0]), 0);
	lastMs = 0 - REFRESH_PERIOD;
}

static uint32_t prevMs;
static int iDS18;
static PollData *pd;

bool DS18_setup()
{
	pd = (PollData*)malloc(sizeof(PollData)*nDS18);
	for (int i = 0; i < nDS18; i++)
		pd[i] = PollData(ds18pin[i]);
	prevMs = 0 - POLL_PERIOD;
	iDS18 = 0;
}

const char sKind[] = "DS18B";

bool DS18_loop()
{
	if (millis() - prevMs < POLL_PERIOD)
		return false;
	prevMs = millis();
	ds.setOneWire(&(pd[iDS18].wire));
	ds.requestTemperatures();
	uint16_t t16 = ds.getTemp(&(pd[iDS18].addr[0]));
	char *sCode = pin_to_dec(ds18pin[iDS18]);
	if (t16 == DEVICE_DISCONNECTED_RAW)
		MQTT_publish(MQTT_topic(sKind, sCode, sError), "n/c");
	else
	{
		char sValue[6];
		float t = ds.rawToCelsius(t16);
		float dT = t - pd[iDS18].prevT;
		if (millis() - pd[iDS18].lastMs > REFRESH_PERIOD || dT < -0.1f || 0.1f < dT)
		{
			pd[iDS18].prevT = t;
			dht[iDS18].lastMs = millis();
			dtostrf(t, 4, 1, sValue);
			MQTT_publish(MQTT_topic(sKind, sCode, "temp"), sValue);
		}
	}

	if (++iDS18 == nDS18)
		iDS18 = 0;
	return true;
}

#endif