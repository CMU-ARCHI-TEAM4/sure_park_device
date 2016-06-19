/***************************************************************************************
 *   LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2016 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 ***************************************************************************************/

/** @file network.h
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
#include <SPI.h>
#include <WiFi.h>

// Server IP & Port number
#define PORTID  7777
#define SERVER_IP   192,168,1,41

// AP SSID and Password
#define AP_SSID     "LGArchi_Guest1"
#define AP_PASSWD   "16swarchitect"

IPAddress server(SERVER_IP);    // The server's IP address
WiFiClient client;              // The client (our) socket

byte wifiMACAddr[6];            // Wifi shield MAC address

void InitNetwork(void);
void PrintConnectStatus(void);
