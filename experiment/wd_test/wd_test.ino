/* Using Watchdog timer in Arduino projects

First of all, you need to include corresponding header file:
#include <avr/wdt.h>

In the next step we should enable WDT as well as configure it:
wdt_enable(WDTO_2S);

In most cases you'll have to put that line to init(){} section, however, Watchdog timer may be enabled at any moment.

WDTO_2S is a constant that means threshold interval of 2 seconds. (It is defined inside header file). If Watchdog receive no "pat" signal for this period, it will "bite" back - reboot the MCU.

Threshold values are pre-programmed (you cannot set any value you wish).
Threshold value     Constant name   Supported on
15 ms   WDTO_15MS   ATMega 8, 168, 328, 1280, 2560
30 ms   WDTO_30MS   ATMega 8, 168, 328, 1280, 2560
60 ms   WDTO_60MS   ATMega 8, 168, 328, 1280, 2560
120 ms  WDTO_120MS  ATMega 8, 168, 328, 1280, 2560
250 ms  WDTO_250MS  ATMega 8, 168, 328, 1280, 2560
500 ms  WDTO_500MS  ATMega 8, 168, 328, 1280, 2560
1 s     WDTO_1S     ATMega 8, 168, 328, 1280, 2560
2 s     WDTO_2S     ATMega 8, 168, 328, 1280, 2560
4 s     WDTO_4S     ATMega 168, 328, 1280, 2560
8 s     WDTO_8S     ATMega 168, 328, 1280, 2560

Finally, you should put the following line that tells watchdog that everything's ok.
wdt_reset();
*/

/* timer interrupt
https://arduino-info.wikispaces.com/Timers-Arduino
*/

#include <avr/wdt.h>


#define DELAY_1S 1000 //ms

void setup()
{
    InitEntryExitLEDs(); // You have to do this to turn off the 

    // Initialize the Serial port:
    Serial.begin(9600);

    Serial.println( "#######################" );  //Here we close both gates
    Serial.println( "Test Watchdog timer!!~~" );  //Here we close both gates
    Serial.println( "#######################" );  //Here we close both gates
    wdt_enable(WDTO_4S);

}

void loop()
{
    delay( DELAY_1S );
    Serial.print( "be about to reset by watchdog timer :" );  //Here we close both gates
    Serial.println(3);
    Serial.println(millis());
    delay( DELAY_1S );
    Serial.print( "be about to reset by watchdog timer :" );  //Here we close both gates
    Serial.println(2);
    Serial.println(millis());
    delay( DELAY_1S );
    Serial.print( "be about to reset by watchdog timer :" );  //Here we close both gates
    Serial.println(1);
    delay( DELAY_1S );
    Serial.println(millis());
    Serial.println( "kick the watchdog" );  //Here we close both gates
    wdt_reset();
}

/*********************************************************************
 * void InitEntryExitLEDs()
 *
 * Parameters: None           
 * 
 * Description:
 * The entry and exit LEDs are 3 way LEDs with a common annode. This means
 * that you pull the other legs low to lite the appropriate colored LED.
 * The problem is that when you turn on the CPU, the pins are typically low
 * meaning that the LEDs will be on. This method, simply ensures they are 
 * off.
 ***********************************************************************/
void InitEntryExitLEDs()
{
    int i;
    for (i=26; i<=29; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
}

