#include "stdafx.h"
#include "eda.h"

#define PINGECHOTIME		(15000/200) // (15sec/200millisecond)

enum WIFICMDSTATUS
{
	eNOACTION,
	eCONNECTION,
	eDEVICEINFO,
	ePINGECHO,
	eENTRYSENSOR,
	eCONFIRMATION,
	eENTRYGATE,
	ePARKING,
	eEXITGATE,
	eREADDATA
};


Wireless::Wireless(char * ssid, char * password, IPAddress server, int portId)
{
	wifi_status = WL_IDLE_STATUS;

	while (wifi_status != WL_CONNECTED) {
		Serial.println("Attempting to connect to WPA SSID");
		wifi_status = WiFi.begin(ssid, password);
		delay(1000);
	}
	Serial.println("WL_CONNECTED");

	ip = WiFi.localIP();
	subnet = WiFi.subnetMask();
	WiFi.macAddress(mac);
	rssi = WiFi.RSSI();
	encryption = WiFi.encryptionType();
	serverIp = server;
	serverPort = portId;
	waitTimeforConnection = 10000 / 200;
	serverConnected = 0;
	myName = WIFI; 
	pingEchoTime = PINGECHOTIME; // (15sec/200millisecond)
}

Wireless::~Wireless()
{
}

unsigned char Wireless::dispatcher(String message)
{
	//TODO : insert dispatcher
	if (compare(message,myName) == 0) {
		delMassge(message, myName, 1);
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

void Wireless::engine(String * message)
{
	String sendMsg;
	macaddress(&sendMsg);

	switch (actionCheck(message)) {
	case eNOACTION :
		break;
	case eCONNECTION:
		break;
	case eDEVICEINFO:
		macaddress(&sendMsg);
		Serial.println(sendMsg + "4");
		client.println(sendMsg + "4");
		break;
	case ePINGECHO:
		Serial.println("PINGECHO enabled\n");
		break;
	case eENTRYSENSOR:
		break;
	case eCONFIRMATION:
		break;
	case eENTRYGATE:
	case eEXITGATE:
		Serial.println(sendMsg+(*message));
		client.println(sendMsg+(*message));
		break;
	case ePARKING:
		Serial.println(sendMsg+(*message));
		client.println(sendMsg+(*message));
		break;
	case eREADDATA:
		while (client.available()) {
			char c = client.read();
			Serial.write(c);
		}
		break;
	default :
		break;
	}
}


int Wireless::actionCheck(String * message)
{
	if (client.connected()) {
		if (pCompare(message, ENTRYSENSOR) == 0)
			return eENTRYSENSOR;
		if (pCompare(message, ENTRYGATE) == 0)
			return eENTRYGATE;
		if (pCompare(message, EXITGATE) == 0)
			return eEXITGATE;
		if (pCompare(message, PARKING) == 0)
			return ePARKING;
		if (pingEchoTime)
			pingEchoTime--;
		else {
			pingEchoTime = PINGECHOTIME;
			return ePINGECHO;
		}
		return eREADDATA;
	}
	else {
		if (serverConnected) {
			client.stop();
			serverConnected = 0;
		}
		else {
			client.connect(serverIp, serverPort);
			serverConnected = 1;
			Serial.println("server is connected");
			return eDEVICEINFO;
		}
	}
	return eNOACTION;
}

void Wireless::macaddress(String * sendmsg)
{
	*sendmsg += "0 ";
	*sendmsg += mac[5];
	*sendmsg += ":";
	*sendmsg += mac[4];
	*sendmsg += ":";
	*sendmsg += mac[3];
	*sendmsg += ":";
	*sendmsg += mac[2];
	*sendmsg += ":";
	*sendmsg += mac[1];
	*sendmsg += ":";
	*sendmsg += mac[0];
	*sendmsg += " ";
}
