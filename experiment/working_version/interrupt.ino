/***************************************************************************************
 *   LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2016 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 ***************************************************************************************/

/** @file interrupt.ino
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
#include <TimerThree.h>

#define PERIOD_INTERRUPT    1000000 // 1sec

void InitIntTimer3(void)
{
    Timer3.initialize(PERIOD_INTERRUPT);
    Timer3.attachInterrupt(IntServiceTimer3); 
}

void IntServiceTimer3(void)
{
    Serial.print(millis());
    Serial.print(": ");
    Serial.println("Timer3 Service routine");
} 
