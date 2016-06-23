#include "stdafx.h"
#include "eda.h"

GateServo::GateServo(unsigned char pinNum)
	: chatteringTime(2000/200), gateCloseTime(0)
	, flipFlop(0)
{
	gateObj = new Servo ;
	gateObj->attach(pinNum);
	gateNum = pinNum;
	myName = ((pinNum == EntryGateServoPin) ? ENTRYGATE : EXITGATE);
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
	if (0==msgCompare(message,1,myName)){
		engine(&message);
		return 1;
	}
	else if (0 == msgCompare(message, 1, TIMETICK)) {
		message = myName;
		InsStr(message, ENTRYSENSOR);
		engine(&message);
		return 1;
	}
	return 0;
}


void GateServo::engine(String * message)
{
	//TODO : insert engine code
	int state = 0;
	String sndMsg ;

	if(0 == msgCompare(*message,2,OPEN)){
		gateObj->write(Open); 
		digitalWrite(greenLED, LOW);
		digitalWrite(redLED, HIGH);

		pDelMassge(message, 2, 2);
		sndMsg = mergeStr(3, PARKING, message->c_str(), LEDON);
		Event_generator::set_event(sndMsg);

		pDelMassge(message, 1, 1);
		sndMsg = mergeStr(2, ALLPARKING, message->c_str());
		Event_generator::set_event(sndMsg);

		sndMsg = mergeStr(3, WIFI, (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE, OPEN);
		Event_generator::set_event(sndMsg);
	}
	else if (0 == msgCompare(*message,2,CLOSE)) {
		gateObj->write(Close);
		digitalWrite(greenLED, HIGH);
		digitalWrite(redLED, LOW);
		sndMsg = mergeStr(3, WIFI, (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE, CLOSE);
		Event_generator::set_event(sndMsg);
	}
	else if (0 == msgCompare(*message, 2, ENTRYSENSOR)) {
		if (EntryGateServoPin == gateNum) 
			state = digitalRead(EntryBeamRcvr);
		else 
			state = digitalRead(ExitBeamRcvr);

		if ((!chatteringTime) && (sensor_status != state) ) {
			sensor_status = state;
			chatteringTime = 2000/200 ;
			if (state)
				gateCloseTime = 3000 / 200;
			else {
				sndMsg = mergeStr(3, WIFI, (EntryGateServoPin == gateNum) ? ENTRYSENSOR : EXITSENSOR, DETECT);
				Event_generator::set_event(sndMsg);
				if (ExitGateServoPin == gateNum) {
					sndMsg = mergeStr(2, (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE, OPEN);
					Event_generator::set_event(sndMsg);
				} 
				else {
					Serial.println("Car is detected");
				}
			}
		}
		else if (chatteringTime){
			chatteringTime--;
		}

		if (gateCloseTime) {
			gateCloseTime--;
			if (0 == gateCloseTime) {
				sndMsg = mergeStr(2, (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE, CLOSE);
				Event_generator::set_event(sndMsg);
			}
		}
	}
	else if (0 == msgCompare(*message, 2, CONNECTINGLED)) {
		if (0 == msgCompare(*message, 3, LEDON)) {
			digitalWrite(greenLED, HIGH);
			digitalWrite(redLED, LOW);
		}
		else {
			digitalWrite(greenLED, LOW);
			digitalWrite(redLED, HIGH);
		}
	}
}
