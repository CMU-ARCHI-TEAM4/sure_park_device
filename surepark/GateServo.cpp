#include "stdafx.h"
#include "eda.h"


GateServo::GateServo(unsigned char pinNum)
	: chatteringTime(0)
{
	gateObj = new Servo ;
	gateObj->attach(pinNum);
	gateNum = pinNum;
	myName += GATESERVO;
	myName += ((pinNum == EntryGateServoPin) ? ENTRY : EXIT);
	gateObj->write(Close);
	if (EntryGateServoPin == gateNum) {
		pinMode(EntryGateGreenLED, OUTPUT);    
		pinMode(EntryGateRedLED, OUTPUT);
		digitalWrite(EntryGateGreenLED, HIGH);
		digitalWrite(EntryGateRedLED, LOW);

		pinMode(EntryBeamRcvr, INPUT);
		digitalWrite(EntryBeamRcvr, HIGH);
	}
	else {
		pinMode(ExitGateGreenLED, OUTPUT);
		pinMode(ExitGateRedLED, OUTPUT);
		digitalWrite(ExitGateGreenLED, HIGH);
		digitalWrite(ExitGateRedLED, LOW);

		pinMode(ExitBeamRcvr, INPUT);
		digitalWrite(ExitBeamRcvr, HIGH);
	}
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
		if (EntryGateServoPin == gateNum) {
			digitalWrite(EntryGateGreenLED, LOW);
			digitalWrite(EntryGateRedLED, HIGH);
			sndMsg += ENTRYGATE;
			Event_generator::set_event(sndMsg + "0");
		}
		else {
			digitalWrite(ExitGateGreenLED, LOW);
			digitalWrite(ExitGateRedLED, HIGH);
			sndMsg += EXITGATE;
			Event_generator::set_event(sndMsg + "0");
		}
	}
	else if (pCompare(message,OFF)==0) {
		gateObj->write(Close);
		if (EntryGateServoPin == gateNum) {
			digitalWrite(EntryGateGreenLED, HIGH);
			digitalWrite(EntryGateRedLED, LOW);
			sndMsg += ENTRYGATE;
			Event_generator::set_event(sndMsg + "1");
		}
		else {
			digitalWrite(ExitGateGreenLED, HIGH);
			digitalWrite(ExitGateRedLED, LOW);
			sndMsg += EXITGATE;
			Event_generator::set_event(sndMsg + "1");
		}
	}
	else if (pCompare(message,IR_DETECT)==0) {
		if (EntryGateServoPin == gateNum) 
			state = digitalRead(EntryBeamRcvr);
		else 
			state = digitalRead(ExitBeamRcvr);

		if ((!chatteringTime) && (sensor_status != state) ) {
			sensor_status = state;
			chatteringTime = 10;
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
		else {
			chatteringTime--;
		}
	}
}
