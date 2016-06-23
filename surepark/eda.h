#pragma once

#include "arduino.h"
#include "WiFi.h"
#include "SPI.h"
#include "Servo.h"
#include "storedMessage.h"


#ifdef UNITTEST
#define compare(x,y)			x.find(y)
#define delMessage(x,y,z)		x.erase(0, y + z);
#define pCompare(x,y)			x->find(y)
#define pDelMassge(x,y,z)		x->erase(0, y + z);
#else
#define compare(x,y)			x.indexOf(y)
#define delMessage(x,y,z)		x.remove(0, y + z);
#define pCompare(x,y)			x->indexOf(y)
#define pDelMassge(x,y,z)		x->remove(0, y + z);
#endif

inline int msgPoint(String src, int order) {
	int orderLoop, position = 0;
	for (orderLoop = 0; orderLoop < (order - 1); orderLoop++) {
#ifdef UNITTEST
		position = src.find(' ', position);
#else
		position = src.indexOf(' ', position);
#endif
		position++;
	}
	return position;
}

inline int msgCompare(String src, int order, String dest)
{
	int position ;
	position = msgPoint(src, order);
#ifdef UNITTEST
	return (src.substr(position) == dest) ? 0 : 1;
#else
	return (src.substring(position, position+dest.length()) == dest) ? 0 : 1;
#endif
}

#define InsStr(x,y)			do{x +=" ";x += y;}while(0);
#define pInsStr(x,y)			do{*x +=" ";*x += y;}while(0);

inline String mergeStr(int num, ...) {
	va_list ap;
	String sumStr = "";
	char* loopStr;;
#ifdef UNITTEST
	__crt_va_start(ap, num);
	for (loopStr = __crt_va_arg(ap, char*); num > 0; num--, loopStr = __crt_va_arg(ap, char*)) {
		for (; *loopStr != '\0'; loopStr++) sumStr += *loopStr;
		sumStr += " ";
	}
	__crt_va_end(ap);
#else
	va_start(ap, num);
	for (loopStr = va_arg(ap, char*); num > 0; num--, loopStr = va_arg(ap, char*)) {
		for (; *loopStr != '\0'; loopStr++) sumStr += *loopStr;
		if(1 != num ) sumStr += " ";
	}
	va_end(ap);
#endif
	return sumStr;
}

// Arduino device pin set
#define ParkingStall1LED		22
#define ParkingStall2LED		23
#define ParkingStall3LED		24
#define ParkingStall4LED		25
#define EntryGateGreenLED		26
#define EntryGateRedLED			27
#define ExitGateGreenLED		28
#define ExitGateRedLED			29

#define EntryBeamRcvr			34
#define ExitBeamRcvr			35

#define Stall1SensorPin			30
#define Stall2SensorPin			31
#define Stall3SensorPin			32
#define Stall4SensorPin			33
#define EntryGateServoPin		5
#define ExitGateServoPin		6
#define Open					90
#define Close					0

#define OPEN					"0"
#define CLOSE					"1"

#define DETECT					"0"
#define NOTDETECT				"1"

#define	LEDON					"0"
#define LEDOFF					"1"
// Device information
#define msgREQUEST		"0"
#define msgRESPONSE		"1"

#define DEVICEINFO		"0"
#define SERVERCONFIG	"1"
#define HEARTBEAT		"2"
#define ENTRYSENSOR		"3"

#define ENTRYGATE		"5"
#define PARKING			"6"
#define EXITGATE		"7"
#define EXITSENSOR		"8"
#define TIMESYNC		"9"
#define TIMETICK		"a"
#define WIFI			"w"
#define CONSOLE			"c"
#define READWIFI		"r"
#define ALLPARKING		"p"
#define STALLSENS		"s"
#define CONNECTINGLED	"n"
#define EEPROM			"e"
#define EEPROMCNT		"ec"
#define EEPROMMSG		"em"
#define TIMEINFO		"0000-00-00-00-00-00"

class Device
{
public:
	Device();
	~Device();
	virtual unsigned char dispatcher(String message) = 0;
	virtual void engine(String * message) = 0;
	unsigned char id_;
	static unsigned char total_;
protected:
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
	unsigned char greenLED;
	unsigned char redLED;
	unsigned char beamRcv;

	unsigned char sensor_status;
	unsigned char chatteringTime;
	unsigned char gateCloseTime;
	int flipFlop;
};

class ParkingPlace :
	public Device
{
public:
	ParkingPlace(unsigned char num, unsigned char pinNum, unsigned char sensorPinNum);
	~ParkingPlace();
	unsigned char dispatcher(String message);
	void engine(String * message);
protected:
	String myName;
	int ProximityVal(unsigned char Pin);
	int RealEstmate(unsigned char pin);
private:
	unsigned char ledNum;
	unsigned char sensorPin;
	String parkingNum;
	unsigned char presentParkingStatus;
	unsigned char chatteringTime;
	String confirmationID;

	int sumVal[8] = { 0, };
	int cnt = 1;
	int loop = 0;

	int sensorMiddleVal;

	int durationTime;
	int durationSec;
	int stableTime;

	int ticktok;
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
	void connectionCheck();
	void set_event(String msg);
	IPAddress ip;
	IPAddress subnet;
	long rssi;
	byte encryption;
	WiFiClient client;
	IPAddress serverIp;
	int serverPort;
	int serverConnect;
	String myName;
	int pingEchoTime;
	char * routerSSID;
	char * routerPassword;
	String mac;

	int serialMode;

	int actionCheck(String * message);
	void msgHeader(String type, String * sendmsg);
	void readData();
	void sendMessageToWiFi(String message);
};


class msgEEPROM :
	public Device
{
public:
	msgEEPROM();
	~msgEEPROM();
	unsigned char dispatcher(String message);
	void engine(String * message);
protected:
	String myName;
	int connected;
	storedMessage * msgObj;
	int address;
};


class Factory
{
public:
	Factory();
	~Factory();
	Device * createConsoleInstance();
	Device * createGateServoInstance(unsigned char gateName);
	Device * createParkingPlaceInstance(unsigned char num, unsigned char pinNum, unsigned char sensorPinNum);
	Device * createWirelessInstance(char * ssid, char * password, IPAddress server, int portId);
};


class Event_generator
{
public:
	Event_generator();
	~Event_generator();
	static unsigned char set_event(String message);
	static String * get_event();
private:
	static String Buff[0x10];
	static unsigned char readPtr;
	static unsigned char writePtr;
};

class Event_checker
{
public:
	Event_checker();
	~Event_checker();
	static void check();
	static int timeTick;
	static Device * dev_instance[0x10];
};

