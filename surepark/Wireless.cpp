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
	eDEVICEINFO,
	ePINGECHO,
	eENTRYSENSOR,
	eEXITSENSOR,
	eCONFIRMATION,
	eENTRYGATE,
	ePARKING,
	eEXITGATE,
	eREADDATA,
	eSERIALMOD
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
	mac += " ";
}

Wireless::~Wireless()
{
}

unsigned char Wireless::dispatcher(String message)
{
	//TODO : insert dispatcher
	if (compare(message,myName) == 0) {
		delMessage(message, myName.length(), 1);
		engine(&message);
		return 1;
	}
	else if (compare(message,TIME_EVENT) == 0) {
		message = READ_WIFI;
		engine(&message);
		return 1;
	}
	return 0;
}

void Wireless::connectionCheck()
{
	int wifi_status;
	String sendMsg;

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

			msgHeader(msgREQUEST, &sendMsg);
			msgHeader(DEVICEINFO, &sendMsg);
			sendMessageToWiFi(sendMsg + "4");
		}
		else {
			Serial.println("connecting is failed");
			serverConnect = 2800 / 200;
			WiFi.disconnect();
		}
		break;
	case (2000 / 200):	// wifi connect
		wifi_status = WL_IDLE_STATUS;
		while (wifi_status != WL_CONNECTED) {
			Serial.println("Attempting to connect to WPA SSID");
			wifi_status = WiFi.begin(routerSSID, routerPassword);
			if (wifi_status != WL_CONNECTED) delay(1000);
			// if 3 retry failed. time up..... 
		}
		ip = WiFi.localIP();
		subnet = WiFi.subnetMask();
		rssi = WiFi.RSSI();
		encryption = WiFi.encryptionType();
		Serial.println("WL_CONNECTED");
		break;
	case (3000 / 200):
		Serial.println("waiting 3sec...");
		break;
	case (4000 / 200):
		Serial.println("waiting 4sec...");
		break;
	case (5000 / 200):
		Serial.println("waiting 5sec...");
		break;
	case (6000 / 200):
		Serial.println("waiting 6sec...");
		break;
	case (7000 / 200):
		Serial.println("waiting 7sec...");
		break;
	case (8000 / 200):
		Serial.println("waiting 8sec...");
		break;
	case (9000 / 200):
		Serial.println("waiting 9sec...");
		break;
	default:
		break;
	}

	serverConnect -= ((serverConnect)?1:0);
}

void Wireless::engine(String * message)
{
	String sendMsg;

	if (!serialMode) 
		connectionCheck();

	switch (actionCheck(message)) {
	case eNOACTION :
		break;
	case eCONNECTION:
		break;
	case eDEVICEINFO:
		msgHeader(msgREQUEST, &sendMsg);
		msgHeader(DEVICEINFO, &sendMsg);
		sendMessageToWiFi(sendMsg + "4");
		break;
	case ePINGECHO:
		msgHeader(msgREQUEST, &sendMsg);
		sendMsg += "2 ";
		sendMsg += "0";
		sendMessageToWiFi(sendMsg + "2");
		break;
	case eENTRYSENSOR:
		msgHeader(msgREQUEST, &sendMsg);
		sendMessageToWiFi(sendMsg + (*message));
		break;
	case eEXITSENSOR :
		msgHeader(msgREQUEST, &sendMsg);
		sendMessageToWiFi(sendMsg + (*message));
		break;
	case eCONFIRMATION:
		msgHeader(msgRESPONSE, &sendMsg);
		break;
	case eENTRYGATE:
	case eEXITGATE:
		msgHeader(msgREQUEST, &sendMsg);
		sendMessageToWiFi(sendMsg + (*message));
		break;
	case ePARKING:
		msgHeader(msgREQUEST, &sendMsg);
		sendMessageToWiFi(sendMsg + (*message));
		break;
	case eSERIALMOD :
		serialMode = (serialMode) ? 0 : 1;
		if (!serialMode) serverConnect = 10000 / 200;
		break;
	case eREADDATA:
		readData();
		break;
	default :
		break;
	}
}

int Wireless::actionCheck(String * message)
{
	if (pCompare(message, ENTRYSENSOR) == 0)
		return eENTRYSENSOR;
	if (pCompare(message, EXITSENSOR) == 0)
		return eEXITSENSOR;
	if (pCompare(message, ENTRYGATE) == 0)
		return eENTRYGATE;
	if (pCompare(message, EXITGATE) == 0)
		return eEXITGATE;
	if (pCompare(message, PARKING) == 0)
		return ePARKING;
	if (pCompare(message, SERIALMODE) == 0)
		return eSERIALMOD;
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
	String sndMsg;
	char available = 0;
	char c;

	while (client.available()) {
		available = 1;
		c = client.read();
		msg += String(c) ;
	}
	if (available)
		Serial.println(msg);
	if (compare(msg, "0") == 0) { //request
		delMessage(msg, 1, 1);
		if (compare(msg, mac) == 0) {
			delMessage(msg, mac.length(), 0);
			switch (actionCheck(&msg)) {
			case eENTRYGATE:
				delMessage(msg, 2, 2);
				sndMsg = ALLPLACE;
				sndMsg += " ";
				sndMsg += msg;
				Event_generator::set_event(sndMsg);
				Event_generator::set_event(msgENTRYGATEOPEN);
				break;
			default:
				break;
			}
		}
	}
}

void Wireless::sendMessageToWiFi(String message)
{
	Serial.println(message);
	if (!serialMode)client.println(message);
}

void Wireless::msgHeader(String type, String * sendmsg)
{
	*sendmsg = type;
	*sendmsg += mac;
}
