#include "Common.h"

#ifdef USE_DHT

#include "DHT_wrk.h"
#include "MQTT_wrk.h"

const uint16_t POLL_PERIOD = 1000u / nDHT;
const uint32_t REFRESH_PERIOD = 10000ul;
static uint32_t prevMs;
static int iDHT;

static FuncMQTThandler prevHandler;

static bool mqttHandler(char* topic, uint8_t* payload, unsigned int length)
{
	if (topic == NULL) {
		prevMs = millis() - POLL_PERIOD;
		for (int i = 0; i < nDHT; i++)
			dht[i].lastMs = millis() - REFRESH_PERIOD;
	}
	return prevHandler(topic, payload, length);
}

void DHT_setup()
{
	for (int i = 0; i < nDHT; i++)
	{
		dht[i].begin();
		dht[i].lastMs = 0 - REFRESH_PERIOD;
	}
	prevMs = 0 - POLL_PERIOD;
	iDHT = 0;
	prevHandler = MQTT_setHandler(mqttHandler);
}

const char sKind[] = "DHTxx";

bool DHT_loop()
{
	if (millis() - prevMs < POLL_PERIOD)
		return false;
	prevMs = millis();
	
	int i = iDHT;
	if (++iDHT == nDHT)
		iDHT = 0;

	float t = dht[i].readTemperature(true);
	float h = dht[i].readHumidity(false);
	bool needRefresh = millis() - dht[i].lastMs > REFRESH_PERIOD;

	bool nanT = isnan(t);
	if (!needRefresh) {
		if (nanT) {
			if (!isnan(dht[i].prevT))
				needRefresh = true;
		}
		else {
			float dT = t - dht[i].prevT;
			if (!(-0.5f <= dT && dT <= 0.5f))
				needRefresh = true;
		}
	}

	bool nanH = isnan(h);
	if (!needRefresh) {
		if (nanH) {
			if (!isnan(dht[i].prevH))
				needRefresh = true;
		}
		else {
			float dH = h - dht[i].prevH;
			if (!(-1.0f <= dH && dH <= 1.0f))
				needRefresh = true;
		}
	}

	if (!needRefresh)
		return true;

	dht[i].lastMs = millis();
	dht[i].prevT = t;
	dht[i].prevH = h;

	// publish
	char sBuf[16];
	char *sValue;
	char *sCode = pin_to_dec(dht[i]._pin);

	if (nanT)
		sValue = sErrValue;
	else { dtostrf(t, 1, 1, sBuf); sValue = sBuf; }
	MQTT_publish(MQTT_topic(sKind, sCode, "temp"), sValue);

	if (nanH)
		sValue = sErrValue;
	else { dtostrf(h, 1, 1, sBuf); sValue = sBuf; }
	MQTT_publish(MQTT_topic(sKind, sCode, "humi"), sValue);

	return true;
}

#endif
