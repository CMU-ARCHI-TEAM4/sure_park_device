#pragma once
#include "arduino.h"
#include "EEPROM.h"

class storedMessage
{
public:
	storedMessage(int addr,storedMessage * obj);
	~storedMessage();
	storedMessage *preMsg;
private:
	int address;
	int length;
public:
	void setMessage(String * str);
	String getMessage();
};

