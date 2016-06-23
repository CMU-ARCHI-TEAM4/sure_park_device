#include "stdafx.h"
#include "eda.h"


msgEEPROM::msgEEPROM()
	:address(0)
{
	myName = EEPROM;
	msgObj = nullptr;
}


msgEEPROM::~msgEEPROM()
{
}


unsigned char msgEEPROM::dispatcher(String message)
{
	if (0 == msgCompare(message, 1, myName)) {
		engine(&message);
		return 1;
	}
	else if (0 == msgCompare(message, 1, TIMETICK)) {
		engine(&message);
		return 1;
	}
	return 0;
}


void msgEEPROM::engine(String * message)
{
	storedMessage * tempObj = nullptr;
	String sndMsg;

	if (0 == msgCompare(*message, 2, EEPROMCNT)) {
		if (0 == msgCompare(*message, 3, CLOSE)) { //connected
			connected = true;
		}
		else {// disconnected
			connected = false;
		}
	}
	else if (0 == msgCompare(*message, 2, EEPROMMSG)) {
		message->remove(0, 5);
		msgObj = new storedMessage(address, msgObj);
		msgObj->setMessage(message);
	}
	else {
		if (nullptr != msgObj) {
			String str = msgObj->getMessage();
			tempObj = msgObj->preMsg;
			msgObj = msgObj->preMsg;
			delete tempObj;
			Event_generator::set_event(str);
		}
	}
}

