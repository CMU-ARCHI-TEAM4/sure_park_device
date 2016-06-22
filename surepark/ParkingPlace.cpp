#include "stdafx.h"
#include "eda.h"

#define PARKED				1
#define EMPTY				0
#define CHATTERINGTIME		5000/200	

ParkingPlace::ParkingPlace(unsigned char num, unsigned char pinNum, unsigned char sensorPinNum)
	:chatteringTime(0)
{
	long val;
	//myName += PARKINGPLACE;
	myName = PARKING;
	parkingNum = num;
	Serial.println("parking place " + parkingNum);
	ledNum = pinNum;
	sensorPin = sensorPinNum;
	pinMode(ledNum, OUTPUT);
	digitalWrite(ledNum, LOW);

	id_ = total_++;

	val = ProximityVal(sensorPin);
	if (val < 70) presentParkingStatus = 1;
	else presentParkingStatus = 0;
}


ParkingPlace::~ParkingPlace()
{
}

unsigned char ParkingPlace::dispatcher(String message)
{
	if (0 == msgCompare(message, 1, myName)) {
		engine(&message);
	}
	else if (0 == msgCompare(message, 1, ALLPARKING)) {
		engine(&message);
	}
	else if (0 == msgCompare(message, 1, TIMETICK)) {
		message = myName;
		InsStr(message, STALLSENS);
		engine(&message);
		return 1;
	}
	return 0;
}

void ParkingPlace::engine(String * message)
{
	long val;
	unsigned char carIn = EMPTY;
	String sndMsg = WIFI;

	if (0 == msgCompare(*message, 2, parkingNum)) {
		if (0 == msgCompare(*message, 4, LEDON)) {
			digitalWrite(ledNum, HIGH);
		}
		else if (0 == msgCompare(*message, 4, LEDOFF)) {
			digitalWrite(ledNum, LOW);
		}
	}
	else if (0 == msgCompare(*message, 1, ALLPARKING)) {
		if (confirmationID == "") {
			pDelMassge(message, 1, 1);
			confirmationID = *message;
		}
		else if ((EMPTY == presentParkingStatus) && (0 == msgCompare(*message, 2, confirmationID))) {
			confirmationID = "";
			digitalWrite(ledNum, LOW);
		}
	}
	else if (0 == msgCompare(*message, 2, STALLSENS)) {
		if (chatteringTime) {
			chatteringTime--;
		}
		else {
			val = ProximityVal(sensorPin);
			if (val < 65) 
				carIn = PARKED;
			else 
				carIn = EMPTY ;

			if ((presentParkingStatus != carIn )&& (confirmationID != "") ){
				presentParkingStatus = carIn;
				chatteringTime = 2000 / 200;
				InsStr(sndMsg, PARKING);
				InsStr(sndMsg, carIn);
				InsStr(sndMsg, parkingNum);
				InsStr(sndMsg, "0");//charging infomation
				InsStr(sndMsg, confirmationID);
				Event_generator::set_event(sndMsg);
				if (PARKED == carIn) {
					sndMsg = PARKING;
					InsStr(sndMsg, parkingNum);
					InsStr(sndMsg, confirmationID);
					InsStr(sndMsg, LEDOFF);
					Event_generator::set_event(sndMsg);
					sndMsg = ALLPARKING;
					InsStr(sndMsg, confirmationID);
					Event_generator::set_event(sndMsg);
				}
				else {
					confirmationID = "";
				}
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