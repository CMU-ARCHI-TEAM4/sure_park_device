/***************************************************************************************
 *   LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2016 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 ***************************************************************************************/

/** @file working_version.ino 
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
#include "global.h"
#include <Servo.h>

// for contorl a servo motor
Servo EntryGateServo;
Servo ExitGateServo;

extern void InitPin(void);
extern void IntServiceTimer3(void);
extern void InitNetwork(void);

void setup()
{
    //init serial
    Serial.begin(115200);

    delay(DELAY_100MS);

    //init board pin
    InitPin();

    //init timerInterrupt 3
    IntServiceTimer3();

    // init network
    InitNetwork();
}


void loop()
{


}
