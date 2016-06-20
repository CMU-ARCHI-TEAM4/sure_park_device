#pragma once

#include "arduino.h"
#include "WiFi.h"
#include "SPI.h"
#include "Servo.h"

#ifdef UNITTEST
#define compare(x,y)			x.find(y)
#define delMassge(x,y,z)		x.erase(0, y.length() + z);
#define pCompare(x,y)			x->find(y)
#define pDelMassge(x,y,z)		x->erase(0, y.length() + z);
#else
#define compare(x,y)			x.indexOf(y)
#define delMassge(x,y,z)		x.remove(0, y.length() + z);
#define pCompare(x,y)			x->indexOf(y)
#define pDelMassge(x,y,z)		x->remove(0, y.length() + z);
#endif

// device name
#define CONSOLE			"ser"
#define GATESERVO		"gse"
#define PARKINGPLACE	"pla"
#define TIME_EVENT		"tim"
#define WIFI			"wif"

// second device name
#define ENTRY			"entry"
#define EXIT			"exit"
#define IR_DETECT		"beam"
#define STALLSENSOR		"stall"

// WIFI event message
#define READ_WIFI		"wifir"
#define ENTRYSENSOR		"3 "
#define ENTRYGATE		"5 "
#define EXITGATE		"7 "
#define PARKING			"6 "

// value
#define ON				"on"
#define OFF				"off"

// System configuration value
#define EntryGateServoPin 5
#define ExitGateServoPin 6
#define Open  90
#define Close 0

#define ParkingStall1LED  22
#define ParkingStall2LED  23
#define ParkingStall3LED  24
#define ParkingStall4LED  25
#define EntryGateGreenLED 26
#define EntryGateRedLED   27
#define ExitGateGreenLED  28
#define ExitGateRedLED    29

#define EntryBeamRcvr		34
#define ExitBeamRcvr		35

class Event_generator
{
public:
	Event_generator();
	~Event_generator();
	static unsigned char regist_event(String message);
	static String * get_event();
private:
	static String Buff[0x10];
	static unsigned char readPtr;
	static unsigned char writePtr;
};

class Device
{
public:
	Device();
	~Device();
	virtual unsigned char dispatcher(String message) = 0;
	virtual void engine(String * message) = 0 ;
	unsigned char id_;
	static unsigned char total_;
protected:
};

class Event_checker
{
public:
	Event_checker();
	~Event_checker();
};

class Console :
	public Device
{
public:
	Console();
	~Console();
	unsigned char dispatcher(String message);
	void engine(String * message);
};

class GateServo :
	public Device
{
public:
	GateServo(unsigned char pinNum);
	~GateServo();
	unsigned char dispatcher(String message);
	void engine(String * message);
protected:
	// Object of gate class
	Servo * gateObj = nullptr;
	String myName;
private:
	unsigned char gateNum;
protected:
	int detect;
};

class ParkingPlace :
	public Device
{
public:
	ParkingPlace(unsigned char num, unsigned char pinNum);
	~ParkingPlace();
	unsigned char dispatcher(String message);
	void engine(String * message);
protected:
	String myName;
private:
	unsigned char ledNum;
};

class Wireless :
	public Device
{
public:
	Wireless(char * ssid, char * password, IPAddress server, int portId);
	~Wireless();
	int wifi_status;
	unsigned char dispatcher(String message);
	void engine(String * message);
protected:
	IPAddress ip;
	byte mac[6];
	IPAddress subnet;
	long rssi;
	byte encryption;
	WiFiClient client;
	IPAddress serverIp;
	int serverPort;
	int waitTimeforConnection; 
	int serverConnected;
	String myName;
	int pingEchoTime;
	int actionCheck(String * message);
	void macaddress(String * sendmsg);
};

class watch
{
public:
	watch();
	~watch();
	unsigned char dispatcher(String message);
	void engine(String * message);
};


class Factory
{
public:
	Factory();
	~Factory();
	Device * createConsoleInstance() ;
	Device * createGateServoInstance(unsigned char gateName);
	Device * createParkingPlaceInstance(unsigned char num, unsigned char pinNum);
	Device * createWirelessInstance(char * ssid, char * password, IPAddress server, int portId);
};


