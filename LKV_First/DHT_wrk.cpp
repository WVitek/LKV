#include "Common.h"

#ifdef USE_DHT

#include "DHT_wrk.h"
#include "wDHT.h"
#include "MQTT_wrk.h"

const uint16_t POLL_PERIOD = 30000u / nDHT;
const uint32_t REFRESH_PERIOD = 300000ul;
uint32_t prevMs;
int iDHT;

bool DHT_setup()
{
	for (int i = 0; i < nDHT; i++) 
	{
		dht[i].begin();
		dht[i].lastMs = 0 - REFRESH_PERIOD;
	}
	prevMs = 0 - POLL_PERIOD;
	iDHT = 0;
}

const char sKind[] = "DHTxx";

bool DHT_loop()
{
	if (millis() - prevMs < POLL_PERIOD)
		return false;
	prevMs = millis();
	float dT = dht[iDHT].prevT;
	float dH = dht[iDHT].prevH;
	float t = dht[iDHT].readTemperature(true);
	float h = dht[iDHT].readHumidity(false);
	char *sCode = pin_to_dec(dht[iDHT]._pin);
	if (isnan(h) || isnan(t))
		MQTT_publish(MQTT_topic(sKind, sCode, sError), "NaN");
	else
	{
		char sValue[6];
		dT -= t;
		dH -= h;
		if (millis() - dht[iDHT].lastMs > REFRESH_PERIOD || dT < -0.5f || 0.5f < dT || dH < -1.0f || 1.0f < dH)
		{
			dht[iDHT].lastMs = millis();
			dtostrf(t, 4, 1, sValue);
			MQTT_publish(MQTT_topic(sKind, sCode, "temp"), sValue);
			dtostrf(h, 4, 1, sValue);
			MQTT_publish(MQTT_topic(sKind, sCode, "humi"), sValue);
		}
	}

	if (++iDHT == nDHT)
		iDHT = 0;
	return true;
}

#endif