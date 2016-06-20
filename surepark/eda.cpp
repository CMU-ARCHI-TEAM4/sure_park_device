#include "stdafx.h"
#include "eda.h"

Event_generator::Event_generator()
{
}


Event_generator::~Event_generator()
{
}

unsigned char Event_generator::regist_event(String message)
{
	if (compare(message, TIME_EVENT) != 0)
		Serial.println("event:" + message);

	if ((writePtr + 1) == readPtr) {
		Serial.println("Buffer Overflow") ; 
	}
	else {
		Buff[writePtr&0x0F] = message;
		writePtr++;
	}
	return 0;
}


String * Event_generator::get_event()
{
	unsigned char tempReadPtr = readPtr;
	if (readPtr != writePtr) {
		readPtr++;
		return &Buff[tempReadPtr&0x0F];
	}
	return nullptr;
}


Device::Device()
{
	id_ = total_++;
}


Device::~Device()
{
}


Event_checker::Event_checker()
{
}


Event_checker::~Event_checker()
{
}


Factory::Factory()
{
}


Factory::~Factory()
{
}


Device* Factory::createConsoleInstance()
{
	return new Console;
}


Device * Factory::createGateServoInstance(unsigned char gateName)
{
	return new GateServo(gateName);
}

Device * Factory::createParkingPlaceInstance(unsigned char num, unsigned char pinNum)
{
	return new ParkingPlace(num, pinNum);
}

Device * Factory::createWirelessInstance(char * ssid, char * password, IPAddress server, int portId)
{
	return new Wireless(ssid, password, server, portId);
}
