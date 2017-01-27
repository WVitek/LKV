#include "Common.h"

#ifdef USE_DS18

#include "MQTT_wrk.h"
#include <DallasTemperature.h>
#include <OneWire.h>

const uint16_t POLL_PERIOD = 1000u / nPinsDS18;
const uint32_t REFRESH_PERIOD = 10000ul;
static DallasTemperature ds;

class PollData
{
public:
	PollData(uint8_t pin);
	OneWire wire;
	DeviceAddress addr;
	uint32_t lastMs;
	float prevT;
	bool devicePresent;
private:
};

PollData::PollData(uint8_t pin)
	:wire(pin)
{
	ds.setOneWire(&wire);
	devicePresent = ds.getAddress(&(addr[0]), 0);
	if (!devicePresent) {
		DEBUG_PRINT("DS18: OneWire device not found, pin="); DEBUG_PRINTLN(pin);
	}
	lastMs = 0 - REFRESH_PERIOD;
}

static FuncMQTThandler prevHandler;

static uint32_t prevMs;
static int iDS18;
static PollData *pd;

static bool mqttHandler(char* topic, uint8_t* payload, unsigned int length)
{
	if (topic == NULL) {
		prevMs = millis() - POLL_PERIOD;
		for (int i = 0; i < nPinsDS18; i++)
			pd[i].lastMs = millis() - POLL_PERIOD;
	}
	return prevHandler(topic, payload, length);
}

void DS18_setup()
{
	pd = (PollData*)malloc(sizeof(PollData)*nPinsDS18);
	for (int i = 0; i < nPinsDS18; i++)
		pd[i] = PollData(pinsDS18[i]);
	prevMs = 0 - POLL_PERIOD;
	iDS18 = 0;
	prevHandler = MQTT_setHandler(mqttHandler);
}

const char sKind[] = "DS18B";

bool DS18_loop()
{
	if (millis() - prevMs < POLL_PERIOD)
		return false;
	prevMs = millis();
	int i = iDS18;
	if (++iDS18 == nPinsDS18)
		iDS18 = 0;

	uint16_t t16;
	if (pd[i].devicePresent) {
		ds.setOneWire(&(pd[i].wire));
		ds.requestTemperatures();
		t16 = ds.getTemp(&(pd[i].addr[0]));
	}
	else t16 = DEVICE_DISCONNECTED_RAW;

	bool needRefresh = millis() - pd[i].lastMs > REFRESH_PERIOD;

	float t;
	bool nanT = t16 == DEVICE_DISCONNECTED_RAW;
	if (nanT) {
		t = NAN;
		if (!isnan(pd[i].prevT))
			needRefresh = true;
	}
	else {
		t = ds.rawToCelsius(t16);
		if (!needRefresh) {
			float dT = t - pd[i].prevT;
			if (!(-0.1f <= dT && dT <= 0.1f))
				needRefresh = true;
		}
	}


	if (!needRefresh)
		return true;

	pd[i].lastMs = millis();
	pd[i].prevT = t;

	char sBuf[6];
	char *sValue;
	if (nanT)
		sValue = sErrValue;
	else { dtostrf(t, 4, 1, sBuf); sValue = sBuf; }
	char *sCode = pin_to_dec(pinsDS18[i]);
	MQTT_publish(MQTT_topic(sKind, sCode, "temp"), sValue);
	return true;
}

#endif