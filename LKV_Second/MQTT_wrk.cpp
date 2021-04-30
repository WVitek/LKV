#include "MQTT_wrk.h"

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "Common.h"

extern uint8_t ethernetMAC[6];
extern uint8_t ethernetIP[4];

extern uint8_t mqttServerIP[4];
extern uint16_t mqttServerPort;

extern char mqttBuf[];

bool default_handler(char* topic, uint8_t* payload, unsigned int length) { return false; }

static void mqttCallback(char* topic, uint8_t* payload, unsigned int length);

static FuncMQTThandler f_handler = default_handler;
static EthernetClient ethClient;
static PubSubClient mqttClient(mqttServerIP, mqttServerPort, mqttCallback, ethClient);
//static PubSubClient mqttClient(mqttServerIP, mqttServerPort, NULL, ethClient);

// MQTT callback function
static void mqttCallback(char* topic, uint8_t* payload, unsigned int length)
{
	if (f_handler(topic, payload, length) || length == 0)
		return;
	if (topic != NULL) {
		DEBUG_PRINT(topic);
		DEBUG_PRINT('=');
		DEBUG_PRINTLN((char)payload[0]);
	}
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
	//if (mqttClient.connected())
	mqttClient.publish(topic, msg);
	DEBUG_PRINT(topic); DEBUG_PRINT('\t'); DEBUG_PRINTLN(msg);
}

void MQTT_subscribe(char * topic, uint8_t qos)
{
	mqttClient.subscribe(topic, qos);
	//DEBUG_PRINT("MQTT_subscribe to "); DEBUG_PRINTLN(topic);
}

FuncMQTThandler MQTT_setHandler(FuncMQTThandler newCallback)
{
	FuncMQTThandler prev = f_handler;
	f_handler = newCallback;
	return prev;
}

static uint32_t prevMs = -5000;

bool MQTT_connect(char* reason)
{
	if (mqttClient.connected())
		return true; char* buf = MQTT_topic(NULL, NULL, NULL);
	char clientName[3];
	clientName[0] = buf[0];
	clientName[1] = buf[1];
	clientName[2] = '\0';
	DEBUG_PRINT("MQTT: Try connect @"); DEBUG_PRINT(reason);
	if (mqttClient.connect(clientName) != 1)
	{
		DEBUG_PRINTLN(": FAILED");
		return false;
	}
	DEBUG_PRINTLN(": SUCCESS");
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
	else DEBUG_PRINTLN("MQTT: check connection");
	return true;
}


