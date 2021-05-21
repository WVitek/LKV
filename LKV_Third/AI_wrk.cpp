#include "Common.h"

#ifdef USE_AI

#include "MQTT_wrk.h"
#include "AI_wrk.h"

const uint16_t POLL_PERIOD = 1000u / nPinsDS18;
const uint32_t REFRESH_PERIOD = 10000ul;

struct PollData
{
public:
	uint32_t lastMs;
	int16_t prev;
};

static FuncMQTThandler prevHandler;

static uint32_t prevMs;
static int iAI;
static PollData *pd;

static bool mqttHandler(char* topic, uint8_t* payload, unsigned int length)
{
	if (topic == NULL) {
		prevMs = millis() - POLL_PERIOD;
		for (int i = 0; i < nPinsAI; i++)
			pd[i].lastMs = millis() - POLL_PERIOD;
	}
	return prevHandler(topic, payload, length);
}

void AI_setup()
{
	pd = (PollData*)malloc(sizeof(PollData)*nPinsAI);
	for (int i = 0; i < nPinsAI; i++) {
		pinMode(pinsAI[i], INPUT);
		pd[i].lastMs = 0 - POLL_PERIOD;
	}
	prevMs = 0 - POLL_PERIOD;
	iAI = 0;
	prevHandler = MQTT_setHandler(mqttHandler);
}

bool AI_loop()
{
	if (millis() - prevMs < POLL_PERIOD)
		return false;
	prevMs = millis();
	int i = iAI;
	if (++iAI == nPinsAI)
		iAI = 0;

	int16_t value = analogRead(pinsAI[i]);

	bool needRefresh = (millis() - pd[i].lastMs > REFRESH_PERIOD) || value != pd[i].prev;

	if (!needRefresh)
		return true;

	pd[i].lastMs = millis();
	pd[i].prev = value;

	char sBuf[7];
	char *sCode = pin_to_dec(pinsAI[i]);
	char *sValue = itoa(value, sBuf, 10);
	MQTT_publish(MQTT_topic("AINPS", sCode, "raw"), sValue);
	return true;
}

#endif
