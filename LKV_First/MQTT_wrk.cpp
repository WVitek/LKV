#include "MQTT_wrk.h"

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "Common.h"

// Callback function header
void callback(char* topic, uint8_t* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client(mqttServerIP, mqttServerPort, callback, ethClient);

// Callback function
void callback(char* topic, uint8_t* payload, unsigned int length) {
	// In order to republish this payload, a copy must be made
	// as the orignal payload buffer will be overwritten whilst
	// constructing the PUBLISH packet.

	// Allocate the correct amount of memory for the payload copy
	byte* p = (byte*)malloc(length);
	// Copy the payload to the new buffer
	memcpy(p, payload, length);
	client.publish("outTopic", p, length);
	// Free the memory
	free(p);
}

//char to_hex(int x) { return (x<10) ? x + '0' : (x - 10) + 'A'; }

//=============== MQTT =================

char* MQTT_topic(const char kind[5], const char code[2], const char descr[])
{
	//                  01234567890123456890123
	//char mqttBuf[] = "01/kkkkk/cc/description";

	if (kind == NULL)
	{
		mqttBuf[2] = 0; return mqttBuf;
	}
	mqttBuf[2] = '/';
	mqttBuf[3] = kind[0];
	mqttBuf[4] = kind[1];
	mqttBuf[5] = kind[2];
	mqttBuf[6] = kind[3];
	mqttBuf[7] = kind[4];
	mqttBuf[9] = code[0];
	mqttBuf[10] = code[1];
	if (descr == NULL)
		mqttBuf[11] = 0;
	else {
		mqttBuf[11] = '/';
		for (int i = 12; (mqttBuf[i] = descr[i - 12]) != 0 && i<24; i++);
	}
	return mqttBuf;
}

void MQTT_publish(char* topic, char* msg)
{
	if (client.connected())
		client.publish(topic, msg);
	Serial.print(topic); Serial.print('\t'); Serial.println(msg);
}

static uint32_t prevMs = 0;

bool MQTT_connect(char* reason)
{
	char* buf = MQTT_topic(NULL, NULL, NULL);
	char clientName[3];
	clientName[0] = buf[0];
	clientName[1] = buf[1];
	clientName[2] = '\0';
	if (client.connect(clientName) != 1)
	{
		Serial.println("MQTT: Error connecting");
		return false;
	}
	Serial.println("MQTT: Connected");
	client.subscribe(MQTT_topic("DIOUT", "#", NULL));
	client.publish(MQTT_topic("EVENT", "00", "connect"), reason);
	return true;
}

void MQTT_setup()
{
	Ethernet.begin(ethernetMAC, ethernetIP);
	MQTT_connect("setup");
}

bool MQTT_loop()
{
	client.loop();
	if (millis() - prevMs < 10000u)
		return false;
	prevMs = millis();
	if (!client.connected())
		MQTT_connect("loop");
	return true;
}


