#include "stdafx.h"
#include "eda.h"

#define PINGECHOTIME		(15000/200) // (15sec/200millisecond)

#define SUCCESS					1
#define TIMED_OUT				-1
#define INVALID_SERVER			-2
#define TRUNCATED				-3
#define INVALID_RESPONSE		-4

enum WIFICMDSTATUS
{
	eNOACTION,
	eCONNECTION,
	ePINGECHO,
	eENTRYSENSOR,
	eEXITSENSOR,
	eCONFIRMATION,
	eENTRYGATE,
	ePARKING,
	eEXITGATE,
	eREADDATA,
	eSERIALMOD,
	eEEPROM
};


Wireless::Wireless(char * ssid, char * password, IPAddress server, int portId)
{
	byte macByte[6];

	routerSSID = ssid;
	routerPassword = password;
	serverIp = server;
	serverPort = portId;
	myName = WIFI;
	pingEchoTime = PINGECHOTIME; // (15sec/200millisecond)
	serverConnect = 10000/200;
	serialMode = 0;
	
	WiFi.macAddress(macByte);
	mac = macByte[5];
	mac += ":";
	mac += macByte[4];
	mac += ":";
	mac += macByte[3];
	mac += ":";
	mac += macByte[2];
	mac += ":";
	mac += macByte[1];
	mac += ":";
	mac += macByte[0];
}

Wireless::~Wireless()
{
}

unsigned char Wireless::dispatcher(String message)
{
	//TODO : insert dispatcher
	if (0 == msgCompare(message, 1, myName)) {
		engine(&message);
		return 1;
	}
	else if (0 == msgCompare(message, 1, TIMETICK)) {
		message = myName;
		InsStr(message, READWIFI);
		engine(&message);
		return 1;
	}
	return 0;
}

void Wireless::connectionCheck()
{
	int wifi_status;
	String sndMsg = msgREQUEST;
	int retry = 3;

	switch (serverConnect) {
	case 0:
		if (!client.connected()) {
			client.stop();
			serverConnect = 1800 / 200;
		}
		break;
	case (1000 / 200):	// server connect
		if (client.connect(serverIp, serverPort) == SUCCESS) {
			Serial.println("Sever is connected");
			sndMsg = mergeStr(4, msgREQUEST, mac.c_str(), DEVICEINFO, mac.c_str());
			InsStr(sndMsg, Device::total_);
			sendMessageToWiFi(sndMsg);
			pingEchoTime = 0;
			sndMsg = mergeStr(3, ENTRYGATE, CONNECTINGLED, LEDON);
			Event_generator::set_event(sndMsg);
		}
		else {
			Serial.println("connecting is failed");
			serverConnect = 2800 / 200;
			WiFi.disconnect();
		}
		break;
	case (2000 / 200):	// wifi connect
		wifi_status = WL_IDLE_STATUS;
		while ((wifi_status != WL_CONNECTED)&&retry) {
			Serial.println("Attempting to connect to WPA SSID");
			wifi_status = WiFi.begin(routerSSID, routerPassword);
			retry--;
		}
		if (retry == 0) serverConnect = 150;
		else {
			ip = WiFi.localIP();
			subnet = WiFi.subnetMask();
			rssi = WiFi.RSSI();
			encryption = WiFi.encryptionType();
			Serial.println("WL_CONNECTED");
		}
		break;
	default:
		if ((serverConnect % 5) == 0){
			Serial.print("waiting ");
			Serial.print(serverConnect / 5, DEC);
			Serial.print("sec...\n");
		}
		break;
	}
	if (serverConnect>(1000/200)) {
		if ((serverConnect % 4) == 0) {
			if ((serverConnect / 4) % 2) {
				sndMsg = mergeStr(3, ENTRYGATE, CONNECTINGLED, LEDON);
			}else{
				sndMsg = mergeStr(3, ENTRYGATE, CONNECTINGLED, LEDOFF);
			}
			Event_generator::set_event(sndMsg);
		}
	}

	serverConnect -= ((serverConnect)?1:0);
}

void Wireless::engine(String * message)
{
	String sndMsg ;

	if (!serialMode) 
		connectionCheck();

	switch (actionCheck(message)) {
	case ePINGECHO:
		sndMsg = mergeStr(4, msgREQUEST, mac.c_str(), HEARTBEAT, TIMEINFO);
		sendMessageToWiFi(sndMsg);
		break;
	case eENTRYSENSOR:
		pDelMassge(message, 1, 1);
		sndMsg = mergeStr(3, msgREQUEST, mac.c_str(), message->c_str());
		sendMessageToWiFi(sndMsg);
		break;
	case eEXITSENSOR :
		pDelMassge(message, 1, 1);
		sndMsg = mergeStr(3, msgREQUEST, mac.c_str(), message->c_str());
		sendMessageToWiFi(sndMsg);
		break;
	case eCONFIRMATION:
		break;
	case eENTRYGATE:
		pDelMassge(message, 1, 1);
		sndMsg = mergeStr(3, msgREQUEST, mac.c_str(), message->c_str());
		sendMessageToWiFi(sndMsg);
		break;
	case eEXITGATE:
		pDelMassge(message, 1, 1);
		sndMsg = mergeStr(3, msgREQUEST, mac.c_str(), message->c_str());
		sendMessageToWiFi(sndMsg);
		break;
	case ePARKING:
		pDelMassge(message, 1, 1);
		sndMsg = mergeStr(3, msgREQUEST, mac.c_str(), message->c_str());
		sendMessageToWiFi(sndMsg);
		break;
	case eSERIALMOD :
		serialMode = (serialMode) ? 0 : 1;
		if (!serialMode) serverConnect = 10000 / 200;
		break;
	case eEEPROM :
		pDelMassge(message, 1, 1);
		sndMsg = mergeStr(3, msgREQUEST, mac.c_str(), message->c_str());
		sendMessageToWiFi(sndMsg);
	default :
		readData();
		break;
	}
	sndMsg = "";
}

int Wireless::actionCheck(String * message)
{
	if (0 == msgCompare(*message,2, ENTRYSENSOR))
		return eENTRYSENSOR;
	if (0 == msgCompare(*message, 2, EXITSENSOR))
		return eEXITSENSOR;
	if (0 == msgCompare(*message,2, ENTRYGATE))
		return eENTRYGATE;
	if (0 == msgCompare(*message, 2, EXITGATE))
		return eEXITGATE;
	if (0 == msgCompare(*message, 2, PARKING))
		return ePARKING;
	if (0 == msgCompare(*message, 2, CONSOLE))
		return eSERIALMOD;
	if (0 == msgCompare(*message, 2, EEPROMCNT))
		return eEEPROM;
	if (pingEchoTime) {
		pingEchoTime--;
		return eREADDATA;
	}
	else {
		pingEchoTime = PINGECHOTIME;
		return ePINGECHO;
	}

	return eNOACTION;
}

void Wireless::readData()
{
	String msg ="";
	String sndMsg = "";
	char available = 0;
	char c;
	int end;

	while (client.available()) {
		available = 1;
		c = client.read();
#ifdef UNITTEST
		msg += c;
#else
		msg += String(c) ;
#endif
	}
	if (available) {
		if (msg.length()) {
			for (end = msg.indexOf('>'); end != (msg.length() - 1); end = msg.indexOf('>')) {
				sndMsg = msg;
				sndMsg.remove(end);
				if (msgCompare(sndMsg, 3, HEARTBEAT))
					Serial.println(sndMsg);
				set_event(sndMsg);
				msg.remove(0, end);
			}
			msg.remove(end);
			if (msgCompare(msg, 3, HEARTBEAT))
				Serial.println(msg);
			set_event(msg);
		}
	}
}

void Wireless::set_event(String msg)
{
	if (compare(msg, "0") == 0) { //request
		delMessage(msg, 1, 1);
		if (compare(msg, mac) == 0) {
			delMessage(msg, mac.length(), 1);
			Event_generator::set_event(msg);
		}
	}
}

void Wireless::sendMessageToWiFi(String message)
{
	if(msgCompare(message,3, HEARTBEAT))
		Serial.println(message);
	if (!serialMode)client.println(message);
}

void Wireless::msgHeader(String type, String * sendmsg)
{
	*sendmsg = type;
	pInsStr(sendmsg, mac);
}
