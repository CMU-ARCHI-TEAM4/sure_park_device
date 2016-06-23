#include "stdafx.h"
#include "eda.h"

#define CONFIRM_CAR			"confirm"


Console::Console()
{
	//TODO : serial initial
}


Console::~Console()
{
}


unsigned char Console::dispatcher(String message)
{
	//TODO : insert dispatcher
	if (0 == msgCompare(message, 1, CONSOLE)) {
		// in 3 id
		if ( 0 == strncmp(CONFIRM_CAR, message.c_str(), sizeof(CONFIRM_CAR)-1))
			serial_engine(&message);
		else
			engine(&message);
	}
	return 0;
}


void Console::engine(String * message)
{
	//TODO : insert engine code
	//Serial.println(*message);
	pDelMassge(message, 1, 1);
	Event_generator::set_event(*message);
}

void Console::serial_engine(String * message)
{
	String sndMsg;

	message->remove(0, sizeof(CONFIRM_CAR));
	sndMsg = mergeStr(3, ENTRYGATE, OPEN, message->c_str()); 
	Event_generator::set_event(sndMsg);
}

void pinMode(unsigned char pinNum, unsigned char mode);
void digitalWrite(unsigned char pinNum, unsigned char status);
