/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2019 Sensnology AB
 * Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - tekka
 *
 * DESCRIPTION
 * Passive node example: This is a passive & independent reporting node
 *
 */

// Enable debug prints
#define MY_DEBUG

// Enable passive mode
#define MY_PASSIVE_NODE

// Passive mode requires static node ID
#define MY_NODE_ID 100

// Enable and select radio type attached
#define MY_RADIO_RF24
//#define MY_RADIO_NRF5_ESB
//#define MY_RADIO_RFM69
//#define MY_RADIO_RFM95

#include <MySensors.h>

#define CHILD_ID 0   // Id of the sensor child
//bool SensorStat = LOW;
// Initialize general message
MyMessage msg(CHILD_ID, V_TEMP);

void setup()
{
  pinMode(6, INPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}

void presentation()
{
	// Send the sketch version information to the gateway and controller
	sendSketchInfo("SmartWater", "1.0");

	// Register all sensors to gw (they will be created as child devices)
	present(CHILD_ID, S_TEMP);
  
}

void loop()
{
  bool SensorStat = digitalRead(6);
  if (SensorStat == HIGH){
    send(msg.set("Need Water!"));
    send(msg.set(SensorStat));
    digitalWrite(4, HIGH);
    sleep(1000);
    digitalWrite(4, LOW);
  }else{
    digitalWrite(4, LOW);
    send(msg.set(SensorStat));
    sleep(2000);  
  }
	
}
