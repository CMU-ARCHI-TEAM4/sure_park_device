/***************************************************************************************
 *   LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2016 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 ***************************************************************************************/

/** @file init.ino
  *
  *  This file contains 
  *
  *  @author   
  *  @version  
  *  @date     2016.06.19
  *  @note
  *  @see
  *
 **/
#include "pin_layout.h"
#include "global.h"

void InitPin(void)
{
    int i;

    Serial.print(millis());
    Serial.print(": ");
    Serial.println("InitPin Start");
    // Init parking lot LED
    for (i = PIN_PARKING_LED_1; i <= PIN_PARKING_LED_4; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }

    // entry/exit gate LED
    for (i = PIN_ENTRY_RED; i <= PIN_ENTRY_GREEN; i++)
    {
        pinMode(i, OUTPUT);

        if(i == PIN_ENTRY_RED || i == PIN_EXIT_RED)
            digitalWrite(i, LOW);
    }

    // init servo motor to close
    EntryGateServo.attach(PIN_ENTRY_SERVO);
    ExitGateServo.attach(PIN_EXIT_SERVO);
    EntryGateServo.write(CLOSE_GATE); 
    ExitGateServo.write(CLOSE_GATE);  

    Serial.print(millis());
    Serial.print(": ");
    Serial.println("InitPin End");
}
