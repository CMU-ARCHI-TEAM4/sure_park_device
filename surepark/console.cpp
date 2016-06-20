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
#ifdef UNITTEST
	if (message.find(CONSOLE) == 0) {
		message.erase(0, sizeof(CONSOLE)+2);
		engine(&message);
		return 1;
	}
#else
	if (message.indexOf(CONSOLE) != -1) {
		message.remove(0,sizeof(CONSOLE) + 2);
		engine(&message);
		return 1;
	}
#endif // UNITTEST
	return 0;
}


void Console::engine(String * message)
{
	//TODO : insert engine code
#ifdef UNITTEST
	SerialClass::println(*message) ;
#else
	Serial.println(*message);
#endif
	Event_generator::set_event(*message);
}

void pinMode(unsigned char pinNum, unsigned char mode);
void digitalWrite(unsigned char pinNum, unsigned char status);
