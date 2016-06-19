/*
   Test the senosr
*/

// Parking log sensors
#define Stall1SensorPin 30
#define Stall2SensorPin 31
#define Stall3SensorPin 32
#define Stall4SensorPin 33

// sensor value
long  Stall1SensorVal;
long  Stall2SensorVal;
long  Stall3SensorVal;
long  Stall4SensorVal;

// Entry, Exit Rcvr
#define EntryBeamRcvr  34 
#define ExitBeamRcvr   35

int EntryBeamState;
int ExitBeamState;
        
void setup() 
{
  InitEntryExitLEDs();   // You have to do this to turn off the 
                         // entry LEDs
                         
  pinMode(EntryBeamRcvr, INPUT);     // Make entry IR rcvr an input
  digitalWrite(EntryBeamRcvr, HIGH); // enable the built-in pullup

  pinMode(ExitBeamRcvr, INPUT);      // Make exit IR rcvr an input
  digitalWrite(ExitBeamRcvr, HIGH);  // enable the built-in pullup

  Serial.begin(115200);
}
     
void loop()
{
  EntryBeamState = digitalRead(EntryBeamRcvr);  // Here we read the state of the
                                                // entry beam.

  if (EntryBeamState == LOW)  // if EntryBeamState is LOW the beam is broken
  {   
    Serial.println("Entry beam broken");
  } else {
    Serial.println("Entry beam is not broken.");
  }

  ExitBeamState = digitalRead(ExitBeamRcvr);  // Here we read the state of the
                                              // exit beam.  
  if (ExitBeamState == LOW)  // if ExitBeamState is LOW the beam is broken
  {     
    Serial.println("Exit beam broken");
  } else {
    Serial.println("Exit beam is not broken.");
  }

   Stall1SensorVal = ProximityVal(Stall1SensorPin); //Check parking space 1
   Serial.print("  Stall 1 = ");
   Serial.print(Stall1SensorVal);
   
   Stall2SensorVal = ProximityVal(Stall2SensorPin); //Check parking space 2
   Serial.print("  Stall 2 = ");
   Serial.print(Stall2SensorVal);

   Stall3SensorVal = ProximityVal(Stall3SensorPin); //Check parking space 3
   Serial.print("  Stall 3 = ");
   Serial.print(Stall3SensorVal);

   Stall4SensorVal =  ProximityVal(Stall4SensorPin); //Check parking space 4
   Serial.print("  Stall 4 = ");
   Serial.println(Stall4SensorVal);
 
   delay(1000);  
}

/*********************************************************************
* long ProximityVal(int Pin)
* Parameters:            
* int pin - the pin on the Arduino where the QTI sensor is connected.
*
* Description:
* QTI schematics and specs: http://www.parallax.com/product/555-27401
* This method initalizes the QTI sensor pin as output and charges the
* capacitor on the QTI. The QTI emits IR light which is reflected off 
* of any surface in front of the sensor. The amount of IR light 
* reflected back is detected by the IR resistor on the QTI. This is 
* the resistor that the capacitor discharges through. The amount of 
* time it takes to discharge determines how much light, and therefore 
* the lightness or darkness of the material in front of the QTI sensor.
* Given the closeness of the object in this application you will get
* 0 if the sensor is covered
***********************************************************************/
long ProximityVal(int Pin)
{
    long duration = 0;
    pinMode(Pin, OUTPUT);         // Sets pin as OUTPUT
    digitalWrite(Pin, HIGH);      // Pin HIGH
    delay(1);                     // Wait for the capacitor to stabilize

    pinMode(Pin, INPUT);          // Sets pin as INPUT
    digitalWrite(Pin, LOW);       // Pin LOW
    while(digitalRead(Pin))       // Count until the pin goes
    {                             // LOW (cap discharges)
       duration++;                
    }   
    return duration;              // Returns the duration of the pulse
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
