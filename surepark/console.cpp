#include "stdafx.h"
#include "eda.h"


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
		engine(&message);
	}
	else {

	}
	return 0;
}


void Console::engine(String * message)
{
	//TODO : insert engine code
	Serial.println(*message);
	pDelMassge(message, 1, 1);
	Event_generator::set_event(*message);
}

void pinMode(unsigned char pinNum, unsigned char mode);
void digitalWrite(unsigned char pinNum, unsigned char status);
