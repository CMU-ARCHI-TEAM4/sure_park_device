#include "stdafx.h"
#include "eda.h"

#define CHATTERINGTIME		5000/200	

ParkingPlace::ParkingPlace(unsigned char num, unsigned char pinNum, unsigned char sensorPinNum)
	:chatteringTime(0)
{
	long val;
	//myName += PARKINGPLACE;
	myName = num;
	parkingNum = num;
	ledNum = pinNum;
	sensorPin = sensorPinNum;
	pinMode(ledNum, OUTPUT);

	val = ProximityVal(sensorPin);
	if (val < 70) presentParkingStatus = 1;
	else presentParkingStatus = 0;
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
	if (0 == compare(message, ALLPLACE)) {
		delMessage(message, sizeof(ALLPLACE), 0);
		engine(&message);
	}
	else
		engine(&message);
	return 0;
}

void ParkingPlace::engine(String * message)
{
	long val;
	unsigned char carIn = false;
	String sndMsg = WIFI;
	sndMsg += " ";

	if (pCompare(message, ON) == 0) {
		digitalWrite(ledNum, HIGH);
	}
	else if (pCompare(message, OFF) == 0) {
		digitalWrite(ledNum, LOW);
	}
	else if (pCompare(message, STALLSENSOR) == 0) {
		chatteringTime = ~chatteringTime;
		// read stall sensor
	}
	else if (0 == pCompare(message, myName)) {
		digitalWrite(ledNum, HIGH);
		pDelMassge(message, myName.length(), +1);
		confirmationID = *message;
	}
	else if (pCompare(message, TIME_EVENT) == 0) {
		if (chatteringTime) {
			chatteringTime--;
		}
		else {
			val = ProximityVal(sensorPin);
			if (val < 65) 
				carIn = 1;
			else 
				carIn = 0;

			if (presentParkingStatus != carIn) {
				presentParkingStatus = carIn;
				if (carIn) {
					Event_generator::set_event(myName+" off");
				}
				chatteringTime = 2000 / 200;
				sndMsg += PARKING;
				sndMsg += carIn;
				sndMsg += " ";
				sndMsg += myName;
				sndMsg += " ";
				sndMsg += "0 ";// charging infomation
				sndMsg += confirmationID;
				sndMsg += "0";
				Event_generator::set_event(sndMsg);
			}
		}
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