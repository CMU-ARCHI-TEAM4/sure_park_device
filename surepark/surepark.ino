#include "eda.h"
#include "TimerThree.h"

#define SERVER_IP    192,168,1,41

unsigned char Device::total_ = 0;
String Event_generator::Buff[0x10];
unsigned char Event_generator::readPtr = 0;
unsigned char Event_generator::writePtr = 0 ;
IPAddress server(SERVER_IP);
String inputString = "" ;

Factory * factory = new Factory;
Device * dev_instance[0xF] = { nullptr, };

void timerCallback()
{
  Event_generator::regist_event(TIME_EVENT);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  inputString.reserve(20);
  Timer3.initialize(200000);         // initialize timer1, and set a 1/2 second period
  Timer3.attachInterrupt(timerCallback);

  dev_instance[0] = factory->createConsoleInstance();
  dev_instance[1] = factory->createGateServoInstance(EntryGateServoPin);
  dev_instance[2] = factory->createGateServoInstance(ExitGateServoPin);
  dev_instance[3] = factory->createParkingPlaceInstance(1, ParkingStall1LED);
  dev_instance[4] = factory->createParkingPlaceInstance(2, ParkingStall2LED);
  dev_instance[5] = factory->createParkingPlaceInstance(3, ParkingStall3LED);
  dev_instance[6] = factory->createParkingPlaceInstance(4, ParkingStall4LED);
  dev_instance[7] = factory->createWirelessInstance("LGArchi_Guest1","16swarchitect", server, 7777);
}

void loop(void)
{
  String * message = nullptr;
  Device ** loop = dev_instance;
  if (message = Event_generator::get_event()) {
    for (; *loop != nullptr; loop++) (*loop)->dispatcher(*message) ;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inch = (char)Serial.read();
    if (inch == '\n') {
      // event occurred 
      Event_generator::regist_event("ser " + inputString);
      inputString = "" ;
    }
    else if (inch != '\0'){
      inputString += inch;
    }
  }
}
