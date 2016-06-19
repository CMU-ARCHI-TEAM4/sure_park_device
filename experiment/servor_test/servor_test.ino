/* 
 *  Servo test 
 *  entry/exit gate
 */


#include <Servo.h> 

#define EntryGateServoPin 5
#define ExitGateServoPin 6
#define Open  90
#define Close 0

int delayvalue = 1000;
Servo EntryGateServo;
Servo ExitGateServo;

void setup() 
{ 
    InitEntryExitLEDs(); // You have to do this to turn off the 
    // entry LEDs

    // Initialize the Serial port:
    Serial.begin(9600);

    // Map servo to pin
//    EntryGateServo.attach(EntryGateServoPin);
//    ExitGateServo.attach(ExitGateServoPin);
} 

void loop() 
{
    EntryGateServo.attach(EntryGateServoPin);
    ExitGateServo.attach(ExitGateServoPin);

    Serial.println( "Close Both Gates" );  //Here we close both gates
    EntryGateServo.write(Close); 
    ExitGateServo.write(Close);  
    delay( delayvalue );

    Serial.println( "Open Entry Gate" );   //Here we open the entry gate
    EntryGateServo.write(Open);
    delay( delayvalue );

    Serial.println( "Close Entry Gate" );  //Here we close the entry gate
    EntryGateServo.write(Close);
    delay( delayvalue );

    Serial.println( "Open Exit Gate" );    //Here we open the exit gate
    ExitGateServo.write(Open);
    delay( delayvalue );

    Serial.println( "Close Exit Gate" );   //Here we close the exit gate
    ExitGateServo.write(Close);
    delay( delayvalue );
    delay( delayvalue );
    delay( delayvalue );
    delay( delayvalue );

    Serial.println( "Detach the entry gate." );   //Here we close the exit gate
    EntryGateServo.detach();
    delay( 10000);
    delay( 10000);

    Serial.println( "Detach the entry gate." );   //Here we close the exit gate
    ExitGateServo.detach();
    delay( 10000);
    delay( 10000);

    Serial.println( "End detach the gate" );   //Here we close the exit gate
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

