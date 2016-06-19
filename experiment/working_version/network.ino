/***************************************************************************************
 *   LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2016 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 ***************************************************************************************/

/** @file network.ino
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
#include "network.h"

void InitNetwork(void)
{
    int status = WL_IDLE_STATUS;      // Network connection status

    Serial.print(millis());
    Serial.print(": ");
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.print("SSID: ");
    Serial.println(AP_SSID);
    Serial.print("passwd: ");
    Serial.println(AP_PASSWD);

    do {
        status = WiFi.begin(AP_SSID, AP_PASSWD);
        delay(DELAY_500MS);
    } while (status != WL_CONNECTED);

    Serial.print(millis());
    Serial.print(": ");
    Serial.print("Connected to networks");

    PrintConnectStatus();
}


void PrintConnectStatus(void) 
{
    IPAddress ip;                     // The IP address of the shield
    IPAddress subnet;                 // The IP address of the shield
    long rssi;                        // Wifi shield signal strength

    // Print the basic connection and network information: Network, IP, and Subnet mask
    ip = WiFi.localIP();
    Serial.print("Connected to ");
    Serial.print(AP_SSID);
    Serial.print(" IP Address:: ");
    Serial.println(ip);
    subnet = WiFi.subnetMask();
    Serial.print("Netmask: ");
    Serial.println(subnet);

    // Print our MAC address.
    WiFi.macAddress(wifiMACAddr);
    Serial.print("WiFi Shield MAC address: ");
    Serial.print(wifiMACAddr[5],HEX);
    Serial.print(":");
    Serial.print(wifiMACAddr[4],HEX);
    Serial.print(":");
    Serial.print(wifiMACAddr[3],HEX);
    Serial.print(":");
    Serial.print(wifiMACAddr[2],HEX);
    Serial.print(":");
    Serial.print(wifiMACAddr[1],HEX);
    Serial.print(":");
    Serial.println(wifiMACAddr[0],HEX);

    // Print the wireless signal strength:
    rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI): ");
    Serial.print(rssi);
    Serial.println(" dBm");

    // print the encryption type:
    byte encryption = WiFi.encryptionType();
    Serial.print("Encryption Type:");
    Serial.println(encryption,HEX);
    Serial.println();
} 
