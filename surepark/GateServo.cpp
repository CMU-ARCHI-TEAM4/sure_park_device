#include "stdafx.h"
#include "eda.h"


GateServo::GateServo(unsigned char pinNum)
	: chatteringTime(2000/200), gateCloseTime(0)
{
	gateObj = new Servo ;
	gateObj->attach(pinNum);
	gateNum = pinNum;
	myName += GATESERVO;
	myName += ((pinNum == EntryGateServoPin) ? ENTRY : EXIT);
	gateObj->write(Close);
	if (EntryGateServoPin == gateNum) {
		greenLED = EntryGateGreenLED;
		redLED = EntryGateRedLED;
		beamRcv = EntryBeamRcvr;
	}
	else {
		greenLED = ExitGateGreenLED;
		redLED = ExitGateRedLED;
		beamRcv = ExitBeamRcvr;
	}


	pinMode(greenLED, OUTPUT);
	pinMode(redLED, OUTPUT);
	digitalWrite(greenLED, HIGH);
	digitalWrite(redLED, LOW);

	pinMode(beamRcv, INPUT);
	digitalWrite(beamRcv, HIGH);
}


GateServo::~GateServo()
{
	delete gateObj;
}


unsigned char GateServo::dispatcher(String message)
{
	//TODO : insert dispatcher
	if (compare(message,myName) == 0) {
		delMessage(message, myName.length(), 1);
		engine(&message);
		return 1;
	}
	else if (compare(message,TIME_EVENT) == 0) {
		message = IR_DETECT;
		engine(&message);
		return 1;
	}
	return 0;
}


void GateServo::engine(String * message)
{
	//TODO : insert engine code
	int state = 0;
	String sndMsg = WIFI;
	sndMsg += " ";

	if (pCompare(message,ON)==0) {
		gateObj->write(Open); 
		digitalWrite(greenLED, LOW);
		digitalWrite(redLED, HIGH);
		sndMsg += (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE;
		Event_generator::set_event(sndMsg + "0");
	}
	else if (pCompare(message,OFF)==0) {
		gateObj->write(Close);
		digitalWrite(greenLED, HIGH);
		digitalWrite(redLED, LOW);
		sndMsg += (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE;
		Event_generator::set_event(sndMsg + "1");
	}
	else if (pCompare(message,IR_DETECT)==0) {
		if (EntryGateServoPin == gateNum) 
			state = digitalRead(EntryBeamRcvr);
		else 
			state = digitalRead(ExitBeamRcvr);

		if ((!chatteringTime) && (sensor_status != state) ) {
			sensor_status = state;
			chatteringTime = 2000/200 ;
			if (state)
				gateCloseTime = 5000 / 200;
			else 
				if (EntryGateServoPin == gateNum) {
					sndMsg += ENTRYSENSOR;
					Event_generator::set_event(sndMsg + "0");
				}
				else {
					sndMsg += EXITSENSOR;
					Event_generator::set_event(sndMsg + "0");
					Event_generator::set_event(msgEXITGATEOPEN);
				}
		}
		else if (chatteringTime){
			chatteringTime--;
		}

		if (gateCloseTime) {
			gateCloseTime--;
			if (0 == gateCloseTime) {
				sndMsg += (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE;
				Event_generator::set_event((EntryGateServoPin == gateNum)?msgENTRYGATECLOSE:msgEXITGATECLOSE);
			}
		}
	}
}
