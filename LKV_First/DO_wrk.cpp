#include "DO_wrk.h"
#include "Common.h"
#include "MQTT_wrk.h"

static FuncMQTThandler prevHandler;

const char* sDOUTS = "DOUTS";

bool mqttHandler(char* topic, uint8_t* payload, unsigned int length)
{
	if (topic == NULL) 
		// after reconnect we need update subscribes
		MQTT_subscribe(MQTT_topic(sDOUTS, "#", NULL), MQTT_QOS1);
	else if (strlen(topic) == 10 && length == 1)// && strncmp(topic + 3, sDOUTS, 5) == 0)
	{
		//                 01234567890
		// topic format = "01/DOUTS/cc";
		int pin = dec_to_pin(topic + 9);
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