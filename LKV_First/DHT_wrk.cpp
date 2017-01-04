#include "Common.h"

#ifdef USE_DHT

#include "DHT_wrk.h"
#include "wDHT.h"
#include "MQTT_wrk.h"

const uint16_t Period = 30000 / nDHT;
uint32_t prevMs;
int iDHT;

bool DHT_setup()
{
	for (int i = 0; i < nDHT; i++)
		dht[i].begin();
	prevMs = 0;
	iDHT = 0;
}

const char sKind[] = "DHTxx";

bool DHT_loop()
{
	if (millis() - prevMs < Period)
		return false;
	prevMs = millis();
	float t = dht[iDHT].readTemperature(true);
	float h = dht[iDHT].readHumidity(false);
	char *sCode = pin_to_dec(dht[iDHT]._pin);
	if (isnan(h) || isnan(t))
		MQTT_publish(MQTT_topic(sKind, sCode, sError), "NaN");
	else
	{
		char sValue[6];
		dtostrf(t, 4, 1, sValue);
		MQTT_publish(MQTT_topic(sKind, sCode, "temp"), sValue);
		dtostrf(h, 4, 1, sValue);
		MQTT_publish(MQTT_topic(sKind, sCode, "humi"), sValue);
	}

	if (++iDHT == nDHT)
		iDHT = 0;
	return true;
}

#endif