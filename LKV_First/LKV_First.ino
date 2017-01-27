#include "AI_wrk.h"
#include <SPI.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#include "Common.h"

// the setup function runs once when you press reset or power the board
void setup() 
{
	common_setup();
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	common_loop();
}
