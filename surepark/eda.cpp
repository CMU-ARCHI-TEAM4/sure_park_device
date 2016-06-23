#include "stdafx.h"
#include "eda.h"


Device::Device()
{
}

Device::~Device()
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

Device * Factory::createParkingPlaceInstance(unsigned char num, unsigned char pinNum, unsigned char sensorPinNum)
{
	return new ParkingPlace(num, pinNum, sensorPinNum);
}

Device * Factory::createWirelessInstance(char * ssid, char * password, IPAddress server, int portId)
{
	return new Wireless(ssid, password, server, portId);
}


unsigned char Device::total_ = 0;

Event_generator::Event_generator()
{
}

Event_generator::~Event_generator()
{
}

unsigned char Event_generator::set_event(String message)
{
	if (compare(message, TIMETICK))
		Serial.println("set event:" + message);

	if ((writePtr + 1) == readPtr) {
		Serial.println("Bacuse of buffer overflow, Data will be lost.") ; 
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

Event_checker::Event_checker()
{
}

Event_checker::~Event_checker()
{
}

void Event_checker::check()
{
	String * message = nullptr;
	Device ** loop = dev_instance;
	if (message = Event_generator::get_event()) {
		//Serial.println("get event:" + *message);
		for (; *loop != nullptr; loop++) (*loop)->dispatcher(*message);
	}

	if (timeTick) {
		Event_generator::set_event(TIMETICK);
		timeTick = false;
	}
}
