/***************************************************************************************
 *   LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2016 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 ***************************************************************************************/

/** @global.h
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
#include <Servo.h>

// delay function delay
#define DELAY_100MS 100
#define DELAY_500MS 500
#define DELAY_1S    1000
#define DELAY_10S   10000

// Servo motor control value
#define OPEN_GATE   90
#define CLOSE_GATE  0

// for contorl a servo motor
extern Servo EntryGateServo;
extern Servo ExitGateServo;
