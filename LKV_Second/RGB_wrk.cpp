#include "Common.h"

#ifdef USE_RGB

#include "RGB_wrk.h"
#include "MQTT_wrk.h"

const char* sRGBWW = "RGBWW";

static FuncMQTThandler prevHandler;

static bool mqttHandler(char* topic, uint8_t* payload, unsigned int length)
{
	size_t len = topic == NULL ? 0 : strlen(topic);
	DEBUG_PRINT(len); DEBUG_PRINT('/'); DEBUG_PRINTLN(length);
	if (topic == NULL)
		// after reconnect we need update subscribes
		MQTT_subscribe(MQTT_topic(sRGBWW, "#", NULL), 0);
	else if (len == 11 && length <= 10 && strncmp(topic + 3, sRGBWW, 5) == 0)
	{
		//                 01234567890
		// topic format = "01/RGBWW/cc";
		int ch = dec_to_pin(topic + 9);
		if (ch >= nRGB)
			return true;

		DEBUG_PRINT("RGB"); DEBUG_PRINT(ch); DEBUG_PRINT('=');
		for (int i = 0; i < length; i++)
			DEBUG_PRINT((char)payload[i]);
		// payload format = "RRGGBBW1W2" in hex
		rgb[ch].set((char*)payload);
		return true;
	}
	return prevHandler(topic, payload, length);
}

void RGB_setup() {
	prevHandler = MQTT_setHandler(mqttHandler);
	for (int i = 0; i < nRGB; i++)
		rgb[i].setup();
}

RGB::RGB(uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t pinW1, uint8_t pinW2) {
	pins[0] = pinR;
	pins[1] = pinG;
	pins[2] = pinB;
	pins[3] = pinW1;
	pins[4] = pinW2;
}

void RGB::setup() {
	for (int i = 0; i < 5; i++)
		if (pins[i] > 0) {
			pinMode(pins[i], OUTPUT);
			analogWrite(pins[i], 0);
		}
}

void RGB::set(char hex[]) {
	// payload format = "RRGGBBW1W2" in hex
	for (int i = 0; i < 4; i++) {
		if (pins[i] == 0)
			continue;
		int duty = hex_to_int(hex[i * 2]) << 4 + hex_to_int(hex[i * 2 + 1]);
		if (duty < 0)
			break;
		analogWrite(pins[i], duty);
	}
}

#endif
