#include "Common.h"

#ifdef USE_DO

#include "DO_wrk.h"
#include "MQTT_wrk.h"

const char* sDOUTS = "DOUTS";

static FuncMQTThandler prevHandler;

static bool mqttHandler(char* topic, uint8_t* payload, unsigned int length)
{
	//DEBUG_PRINTLN("DO_wrk::mqttHandler");
	size_t len = topic == NULL ? 0 : strlen(topic);
	DEBUG_PRINT(len); DEBUG_PRINT('/'); DEBUG_PRINTLN(length);
	if (topic == NULL)
		// after reconnect we need update subscribes
		MQTT_subscribe(MQTT_topic(sDOUTS, "#", NULL), 0);
	else if (len == 11 && length == 1 && strncmp(topic + 3, sDOUTS, 5) == 0)
	{
		//                 01234567890
		// topic format = "01/DOUTS/cc";
		int pin = dec_to_pin(topic + 9);
		DEBUG_PRINT("DO"); DEBUG_PRINT(pin); DEBUG_PRINT('='); DEBUG_PRINTLN((char)payload[0]);
		pinMode(pin, OUTPUT);
		if (payload[0] == '0')
		{
			digitalWrite(pin, LOW);
			return true;
		}
		else if (payload[0] == '1')
		{
			digitalWrite(pin, HIGH);
			return true;
		}
	}
	return prevHandler(topic, payload, length);
}

void DO_setup()
{
	prevHandler = MQTT_setHandler(mqttHandler);
}

#endif