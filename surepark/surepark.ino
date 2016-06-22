#include "eda.h"
#include "TimerThree.h"

#define SERVER_IP    192,168,1,41

String Event_generator::Buff[0x10];
unsigned char Event_generator::readPtr = 0;
unsigned char Event_generator::writePtr = 0 ;

Device * Event_checker::dev_instance[0x10] = { nullptr, };
int Event_checker::timeTick = false;

IPAddress server(SERVER_IP);

Factory * factory = new Factory;
Device * dev_instance[0xF] = { nullptr, };

String inputString ="" ;

void timerCallback()
{
  Event_checker::timeTick = true;
}

void setup(void)
{
  Serial.begin(9600);
  inputString.reserve(20);
  Timer3.initialize(200000);         // initialize timer1, and set a 1/2 second period
  Timer3.attachInterrupt(timerCallback);

  Event_checker::dev_instance[0] = factory->createConsoleInstance();
  Event_checker::dev_instance[1] = factory->createGateServoInstance(EntryGateServoPin);
  Event_checker::dev_instance[2] = factory->createGateServoInstance(ExitGateServoPin);
  Event_checker::dev_instance[3] = factory->createParkingPlaceInstance(1, ParkingStall1LED, Stall1SensorPin);
  Event_checker::dev_instance[4] = factory->createParkingPlaceInstance(2, ParkingStall2LED, Stall2SensorPin);
  Event_checker::dev_instance[5] = factory->createParkingPlaceInstance(3, ParkingStall3LED, Stall3SensorPin);
  Event_checker::dev_instance[6] = factory->createParkingPlaceInstance(4, ParkingStall4LED, Stall4SensorPin);
  Event_checker::dev_instance[7] = factory->createWirelessInstance("LGArchi_Guest1","16swarchitect", server, 7777);
}

void loop(void)
{
  Event_checker::check();
}

void serialEvent() {
  while (Serial.available()) {
    char inch = (char)Serial.read();
    if (inch == '\n') {
      Event_generator::set_event("c " + inputString);
      inputString = "" ;
    }
    else if (inch != '\0')
      inputString += inch;
  }
}

