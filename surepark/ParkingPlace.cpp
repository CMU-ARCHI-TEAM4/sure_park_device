#include "stdafx.h"
#include "eda.h"

#define SENSOR1				200
#define SENSOR2				90
#define SENSOR3				190
#define SENSOR4				50

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
	ledNum = pinNum;
	sensorPin = sensorPinNum;
	pinMode(ledNum, OUTPUT);
	digitalWrite(ledNum, LOW);
	confirmationID = "";

	switch (num)
	{
	case 1:sensorMiddleVal = SENSOR1;
		break;
	case 2:sensorMiddleVal = SENSOR2;
		break;
	case 3:sensorMiddleVal = SENSOR3;
		break;
	case 4:sensorMiddleVal = SENSOR4;
		break;
	default : sensorMiddleVal = SENSOR1;
		break;
	}

	id_ = total_++;

	durationSec = 0;
	durationTime = 0;
	stableTime = 10;
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
	String sndMsg ;

	if (0 == msgCompare(*message, 1, ALLPARKING)) {
		Serial.print(ledNum - 21); Serial.print(' ');
		pDelMassge(message, 1, 1);
		Serial.println(*message + ">");
		if (confirmationID == "") {
			confirmationID = *message;
		}
		else {
			digitalWrite(ledNum, LOW);
			Serial.println(confirmationID + "#");
			if (EMPTY == presentParkingStatus)
				confirmationID = "";
		}
	}
	else if (0 == msgCompare(*message, 2, parkingNum)) {
		if (0 == msgCompare(*message, 4, LEDON)) {
			digitalWrite(ledNum, HIGH);
		}
		else if (0 == msgCompare(*message, 4, LEDOFF)) {
			digitalWrite(ledNum, LOW);
		}
	}
	else if (0 == msgCompare(*message, 2, STALLSENS)) {

		if (PARKED == presentParkingStatus) {
			durationSec++;
			if ((durationSec) && ((60000 / 200) == durationSec)) {
				durationSec = 0;
				durationTime++;
			}
		}

		if (chatteringTime) {
			chatteringTime--;
		}
		else
		{
			val = RealEstmate(sensorPin);
			if (stableTime) {
				stableTime--;
				sensorMiddleVal = val;
			}
			else if  (confirmationID != "")
				carIn = (val < (sensorMiddleVal-30)) ? PARKED : EMPTY;

			if ((presentParkingStatus != carIn )&& (confirmationID != "") ){
				presentParkingStatus = carIn;
				chatteringTime = 2000 / 200;
				sndMsg = mergeStr(6, WIFI, PARKING, (carIn==PARKED)?"1":"0", parkingNum.c_str(), String(durationTime).c_str(), confirmationID.c_str());
				Event_generator::set_event(sndMsg);
				if (PARKED == carIn) {
					durationSec = 0;
					durationTime = 1 ;
					sndMsg = mergeStr(2, ALLPARKING, confirmationID.c_str());
					Event_generator::set_event(sndMsg);
				}
				else {
					confirmationID = "";
				}
			}
		}
	}
}

int ParkingPlace::ProximityVal(unsigned char Pin)
{
	int duration = 0;
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

int ParkingPlace::RealEstmate(unsigned char pin)
{
	int val=0;

	sumVal[loop&0x07] = ProximityVal(pin);
	
	for (int i = cnt; i > 0; i--) {
		val += sumVal[i - 1];
	}

	loop++;
	cnt += ((cnt >= 8) ? 0 : 1);

	return val / cnt;
}
