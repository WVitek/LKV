#include "MQTT_wrk.h"

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "Common.h"

bool default_handler(char* topic, uint8_t* payload, unsigned int length) { return false; }

FuncMQTThandler f_handler = default_handler;

void mqttCallback(char* topic, uint8_t* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient mqttClient(mqttServerIP, mqttServerPort, mqttCallback, ethClient);

// MQTT callback function
void mqttCallback(char* topic, uint8_t* payload, unsigned int length)
{
	if (f_handler(topic, payload, length) || length == 0)
		return;
	// In order to republish this payload, a copy must be made
	// as the orignal payload buffer will be overwritten whilst
	// constructing the PUBLISH packet.
	// Allocate the correct amount of memory for the payload copy
	byte* p = (byte*)malloc(length);
	// Copy the payload to the new buffer
	memcpy(p, payload, length);
	mqttClient.publish("outTopic", p, length);
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
		for (int i = 12; (mqttBuf[i] = descr[i - 12]) != 0 && i < 24; i++);
	}
	return mqttBuf;
}

void MQTT_publish(char* topic, char* msg)
{
	if (mqttClient.connected())
		mqttClient.publish(topic, msg);
	Serial.print(topic); Serial.print('\t'); Serial.println(msg);
}

void MQTT_subscribe(char * topic, uint8_t qos)
{
	mqttClient.subscribe(topic, qos);
}

FuncMQTThandler MQTT_setHandler(FuncMQTThandler newCallback)
{
	FuncMQTThandler prev = f_handler;
	f_handler = newCallback;
	return prev;
}

static uint32_t prevMs = 0;

bool MQTT_connect(char* reason)
{
	char* buf = MQTT_topic(NULL, NULL, NULL);
	char clientName[3];
	clientName[0] = buf[0];
	clientName[1] = buf[1];
	clientName[2] = '\0';
	if (mqttClient.connect(clientName) != 1)
	{
		Serial.println("MQTT: Error connecting");
		return false;
	}
	Serial.println("MQTT: Connected");
	// special form of callback after reconnect to MQTT-server
	mqttCallback(NULL, NULL, 0);
	//mqttClient.subscribe(MQTT_topic("DOUTS", "#", NULL));
	mqttClient.publish(MQTT_topic("EVENT", "00", "connect"), reason);
	return true;
}

void MQTT_setup()
{
	Ethernet.begin(ethernetMAC, ethernetIP);
	MQTT_connect("setup");
}

bool MQTT_loop()
{
	mqttClient.loop();
	if (millis() - prevMs < 5000u)
		return false;
	prevMs = millis();
	if (!mqttClient.connected())
		MQTT_connect("loop");
	return true;
}


