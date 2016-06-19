#include <TimerThree.h>

#define EntryGateGreenLED 26

void setup(void)
{
  InitEntryExitLEDs(); // You have to do this to turn off the 
  //Timer3.initialize(150000);
  Timer3.initialize(1000000);
  Timer3.attachInterrupt(blinkLED); // blinkLED to run every 0.15 seconds
  Serial.begin(115200);
}

int ledState = HIGH;
volatile unsigned long blinkCount = 0; // use volatile for shared variables

void blinkLED(void)
{
  if (ledState == LOW) {
    ledState = HIGH;
    blinkCount = blinkCount + 1;  // increase when LED turns on
  } else {
    ledState = LOW;
  }
  digitalWrite(EntryGateGreenLED, ledState);
  Serial.println(millis()+':');
}


// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
  unsigned long blinkCopy;  // holds a copy of the blinkCount

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
//  noInterrupts();
//  blinkCopy = blinkCount;
//  interrupts();
//  Serial.print(millis()+':');
//  Serial.print("blinkCount = ");
//  Serial.println(blinkCopy);
  delay(100);
}

void InitEntryExitLEDs()
{
    int i;
    for (i=26; i<=29; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
}

