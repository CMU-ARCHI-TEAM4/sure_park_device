#include "stdafx.h"
#include "eda.h"


ParkingPlace::ParkingPlace(unsigned char num, unsigned char pinNum)
{
	myName += PARKINGPLACE;
	myName += num;
	ledNum = pinNum;
	pinMode(ledNum, OUTPUT);
}


ParkingPlace::~ParkingPlace()
{
}

unsigned char ParkingPlace::dispatcher(String message)
{
#ifdef UNITTEST
	if (message.find(myName) == 0) {
		message.erase(0, myName.length() + 1);
		engine(&message);
		return 1;
	}
#else
	if (message.indexOf(myName) == 0) {
		message.remove(0, myName.length() + 1);
		engine(&message);
		return 1;
	}
#endif // UNITTEST
	return 0;
}

void ParkingPlace::engine(String * message)
{
	if (pCompare(message,OFF)==0) {
		digitalWrite(ledNum, HIGH);
	}
	else if (pCompare(message, ON)==0) {
		digitalWrite(ledNum, LOW);
	}
}