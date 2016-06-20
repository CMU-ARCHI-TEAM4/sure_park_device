#include "stdafx.h"
#include "eda.h"


ParkingPlace::ParkingPlace(unsigned char num, unsigned char pinNum, unsigned char sensorPinNum)
{
	myName += PARKINGPLACE;
	myName += num;
	ledNum = pinNum;
	sensorPin = sensorPinNum;
	pinMode(ledNum, OUTPUT);
	sensorVal = ProximityVal(sensorPin);
}


ParkingPlace::~ParkingPlace()
{
}

unsigned char ParkingPlace::dispatcher(String message)
{
	if (0 == compare(message, myName)) {
		delMessage(message, myName.length(), 1);
		engine(&message);
	}
	else if (0 == compare(message, TIME_EVENT)) {
		delMessage(message, sizeof(TIME_EVENT), 1);
		engine(&message);
	}
	return 0;
}

void ParkingPlace::engine(String * message)
{
	long val;
	long gap;
	String str;
	if (pCompare(message,OFF)==0) {
		digitalWrite(ledNum, HIGH);
	}
	else if (pCompare(message, ON)==0) {
		digitalWrite(ledNum, LOW);
	}
	else {
		// read stall sensor
		val = ProximityVal(sensorPin);
		gap = (sensorVal - val)*(sensorVal - val);
		str = myName;
		str += " Sensor Gap = ";
		str += gap;
		Serial.println(str);
	}
}

long ParkingPlace::ProximityVal(unsigned char Pin)
{
	long duration = 0;
	pinMode(Pin, OUTPUT);         // Sets pin as OUTPUT
	digitalWrite(Pin, HIGH);      // Pin HIGH
	delay(1);                     // Wait for the capacitor to stabilize

	pinMode(Pin, INPUT);          // Sets pin as INPUT
	digitalWrite(Pin, LOW);       // Pin LOW
	while (digitalRead(Pin))       // Count until the pin goes
	{                             // LOW (cap discharges)
		duration++;
	}
	return duration;              // Returns the duration of the pulse
}