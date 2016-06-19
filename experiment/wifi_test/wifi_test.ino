/**************************************************************
* File: ClientDemo
* Project: LG Exec Ed Program
* Copyright: Copyright (c) 2016 LG Electronics.
* Versions:
* 1.0 June 2016.
*
* Description:
*
* This program runs on an Arduino processor with a WIFI shield. 
* This program is a client demo. This runs in a loop communicating
* with a server process that also runs in a loop. The protocol is
* that after we connect to a server, this process sends three '\n'
* terminated strings. The final string is "bye\n" which is a 
* signal to the server that we are done sending data. Then
* this process waits for the server to send a single string back. This illustrates basic client
* server connection and two-way communication.
*
* Compilation and Execution Instructions: Only Compile using 
* Arduino IDE VERSION 1.0.4
*
* Parameters: None
*
* Internal Methods: void printConnectionStatus()
*
************************************************************************************************/

#include <SPI.h>
#include <WiFi.h>

// local header
#include "test.h"

#define PORTID  7777               // IP socket port ID
#define SERVER_IP   192,168,1,41

char ssid[] = "LGArchi_Guest1";              // The network SSID for asus ip
char c;                           // Character read from server
int status = WL_IDLE_STATUS;      // Network connection status
char pass[] = "16swarchitect";  // your network password
// IPAddress server(192,168,1,124);  // The server's IP address
IPAddress server(SERVER_IP);  // The server's IP address
WiFiClient client;                // The client (our) socket
IPAddress ip;                     // The IP address of the shield
IPAddress subnet;                 // The IP address of the shield
long rssi;                        // Wifi shield signal strength
byte mac[6];                      // Wifi shield MAC address

//extern void test(void);

void setup()  
{
    // Initialize a serial terminal for debug messages.
    Serial.begin(9600);

    // Attempt to connect to Wifi network.
    while ( status != WL_CONNECTED) 
    { 
        Serial.print("Attempting to connect to WPA SSID: ");
        // Connect to WPA/WPA2 network:    
        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("passwd: ");
        Serial.println(pass);
        status = WiFi.begin(ssid, pass);
        delay(1000);

    }  
    test();
    Serial.println( "Connected to network:" );
    Serial.println( "\n----------------------------------------" );

    // Print the basic connection and network information.
    printConnectionStatus();

    Serial.println( "\n----------------------------------------\n" );

}

void loop() 
{
    // Here we attempt connect to the server on the port specified above

    Serial.print("\nAttempting to connect to server...");

    if (client.connect(server, PORTID)) 
    {
        Serial.println("connected");

        // We write a couple of messages to the server
        client.println("Howdy there big PC...");
        client.println("I am just a little Arduino...");
        client.println("Bye.");

        // Now read a message from the server

        Serial.print("Server Message: ");

        char c = ' ';      
        while ( c!= '\n' )
        {
            if (client.available()) 
            {
                c = client.read();
                Serial.write(c);
            }
        } 

        // That's it. We wait a second, then do it all again.
        client.stop();
        Serial.println( "Done...");
        delay(1000);

    } // if

} //  LOOP

/************************************************************************************************
 * The following method prints out the connection information
 ************************************************************************************************/

void printConnectionStatus() 
{
    // Print the basic connection and network information: Network, IP, and Subnet mask
    ip = WiFi.localIP();
    Serial.print("Connected to ");
    Serial.print(ssid);
    Serial.print(" IP Address:: ");
    Serial.println(ip);
    subnet = WiFi.subnetMask();
    Serial.print("Netmask: ");
    Serial.println(subnet);

    // Print our MAC address.
    WiFi.macAddress(mac);
    Serial.print("WiFi Shield MAC address: ");
    Serial.print(mac[5],HEX);
    Serial.print(":");
    Serial.print(mac[4],HEX);
    Serial.print(":");
    Serial.print(mac[3],HEX);
    Serial.print(":");
    Serial.print(mac[2],HEX);
    Serial.print(":");
    Serial.print(mac[1],HEX);
    Serial.print(":");
    Serial.println(mac[0],HEX);

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

} // printConnectionStatus
