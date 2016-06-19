
#include <swRTC.h>

swRTC rtc; //create a new istance of the lib
void setup()
{
    InitEntryExitLEDs(); // You have to do this to turn off the 

	rtc.stopRTC(); //stop the RTC
	rtc.setTime(14,36,0); //set the time here
	rtc.setDate(18,6,2016); //set the date here
	rtc.startRTC(); //start the RTC
    // Initialize the Serial port:
    Serial.begin(115200);

    Serial.println( "#######################" );  //Here we close both gates
    Serial.println( "Test RTC time!!~~" );  //Here we close both gates
    Serial.println( "#######################" );  //Here we close both gates
}

void loop()
{
    Serial.print(rtc.getHours(), DEC);
    Serial.print(":");
    Serial.print(rtc.getMinutes(), DEC);
    Serial.print(":");
    Serial.print(rtc.getSeconds(), DEC);
    Serial.print(" -- ");
    Serial.print(rtc.getDay(), DEC);
    Serial.print("/");
    Serial.print(rtc.getMonth(), DEC);
    Serial.print("/");
    Serial.print(rtc.getYear(), DEC);
    Serial.print(" (Leap year: ");
    if (rtc.isLeapYear()) {
		Serial.print("yes");
    } else {
        Serial.print("no");
    }
    Serial.println(")");
    Serial.print("Day of week: ");
    Serial.print(rtc.getWeekDay(), DEC);
    Serial.print(" -- ");
    Serial.print("Timestamp: ");
    Serial.println(rtc.getTimestamp(), DEC);
    delay(1000);
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

