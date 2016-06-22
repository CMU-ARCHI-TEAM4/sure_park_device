#include "stdafx.h"
#include "eda.h"

#define msgEXITGATEOPEN			"7 0"
#define msgENTRYGATECLOSE		"5 1"
#define msgEXITGATECLOSE		"7 1"

GateServo::GateServo(unsigned char pinNum)
	: chatteringTime(2000/200), gateCloseTime(0)
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
	String sndMsg = WIFI;
	String secMsg;

	if(0 == msgCompare(*message,2,OPEN)){
		gateObj->write(Open); 
		digitalWrite(greenLED, LOW);
		digitalWrite(redLED, HIGH);

		secMsg = PARKING;
		pDelMassge(message, 2, 2);
		InsStr(secMsg, *message);
		InsStr(secMsg, LEDON);
		Event_generator::set_event(secMsg);

		secMsg = ALLPARKING;
		pDelMassge(message, 1, 1);
		InsStr(secMsg, *message);
		Event_generator::set_event(secMsg);

		InsStr(sndMsg, (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE);
		InsStr(sndMsg, OPEN);
		Event_generator::set_event(sndMsg);
	}
	else if (0 == msgCompare(*message,2,CLOSE)) {
		gateObj->write(Close);
		digitalWrite(greenLED, HIGH);
		digitalWrite(redLED, LOW);
		InsStr(sndMsg, (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE);
		InsStr(sndMsg, CLOSE);
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
				gateCloseTime = 5000 / 200;
			else 
				if (EntryGateServoPin == gateNum) {
					InsStr(sndMsg, ENTRYSENSOR);
					InsStr(sndMsg, DETECT);
					Event_generator::set_event(sndMsg);
				}
				else {
					Event_generator::set_event(msgEXITGATEOPEN);
					InsStr(sndMsg, EXITSENSOR);
					InsStr(sndMsg, DETECT);
					Event_generator::set_event(sndMsg);
				}
		}
		else if (chatteringTime){
			chatteringTime--;
		}

		if (gateCloseTime) {
			gateCloseTime--;
			if (0 == gateCloseTime) {
				InsStr(sndMsg, (EntryGateServoPin == gateNum) ? ENTRYGATE : EXITGATE);
				Event_generator::set_event((EntryGateServoPin == gateNum)?msgENTRYGATECLOSE:msgEXITGATECLOSE);
			}
		}
	}
}
