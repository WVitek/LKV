#include "Common.h"
#include "MQTT_wrk.h"
#ifdef USE_DHT
#include "DHT_wrk.h"
#endif
#ifdef USE_DS18
#include "DS18_wrk.h"
#endif
#ifdef USE_DI
#include "DI_wrk.h"
#endif
#ifdef USE_DO
#include "DO_wrk.h"
#endif

void common_setup()
{
	Serial.begin(9600);
	MQTT_setup();
#ifdef USE_DI
	DI_setup();
#endif
#ifdef USE_DO
	DO_setup();
#endif
#ifdef USE_DHT
	DHT_setup();
#endif
#ifdef USE_DS18
	DS18_setup();
#endif

}

void common_loop()
{
	if (MQTT_loop()) return;
#ifdef USE_DI
	if (DI_loop()) return;
#endif
#ifdef USE_DHT
	if (DHT_loop()) return;
#endif
#ifdef USE_DS18
	if (DS18_loop()) return;
#endif

}

char* pin_to_dec(int pin)
{
	static char buf[] = "00";
	buf[1] = (pin % 10) + '0';
	if (pin > 99) pin = 99;
	buf[0] = (pin / 10) + '0';
	return buf;
}

int dec_to_pin(char *twoCharStr)
{
	char c0 = twoCharStr[0];
	if (c0 < '0' || '9' < c0)
		return -1;
	char c1 = twoCharStr[1];
	if (c1 < '0' || '9' < c1)
		return -1;
	return (c1 - '0') * 10 + (c0 - '0');
}
