#include "Common.h"

#ifdef USE_SERVO

#include "SERVO_wrk.h"
#include "Servo2.h"
#include "MQTT_wrk.h"

const char* sSERVO = "SERVO";
static Servo2 servos[NBR_CHANNELS];

static FuncMQTThandler prevHandler;

static bool mqttHandler(char* topic, uint8_t* payload, unsigned int length)
{
	size_t len = topic == NULL ? 0 : strlen(topic);
	if (topic == NULL)
		// after reconnect we need update subscribes
		MQTT_subscribe(MQTT_topic(sSERVO, "#", NULL), 0);
	else if (len == 11 && 0 <= length && length < 6 && strncmp(topic + 3, sSERVO, 5) == 0)
	{
		//                 01234567890
		// topic format = "01/SERVO/cc";
		int pin = dec_to_pin(topic + 9);
		int i = NBR_CHANNELS - 1;
		uint8_t sPin = 0;
		while (i >= 0)
		{
			sPin = servos[i].GetPin();
			if (pin == sPin || sPin == 0)
				break;
			i--;
		}
		if (i >= 0) 
		{
			payload[length] = 0;
			DEBUG_PRINT("SERVO"); DEBUG_PRINT(pin); DEBUG_PRINT('='); DEBUG_PRINTLN((char*)payload);
			if (sPin == 0 || !servos[i].attached())
			{
				if (length == 0)
					return true;
				servos[i].attach(pin);
			}
			else if (length == 0)
			{
				servos[i].detach();
				return true;
			}
			int value = atoi((char*)payload);
			servos[i].write(value);
		}
		return true;
	}
	return prevHandler(topic, payload, length);
}

void SERVO_setup()
{
	prevHandler = MQTT_setHandler(mqttHandler);
}

#endif
