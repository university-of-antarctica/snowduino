/*--------------------------------------------------------------
  Program:      eth_websrv_LED

  Description:  Arduino web server that serves up a web page
                allowing the user to control an LED
  
  Hardware:     - Arduino Uno and official Arduino Ethernet
                  shield. Should work with other Arduinos and
                  compatible Ethernet shields.
                - LED and resistor in series connected between
                  Arduino pin 2 and GND
                
  Software:     Developed using Arduino 1.0.3 software
                Should be compatible with Arduino 1.0 +
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet

  Date:         11 January 2013
 
  Author:       W.A. Smith, http://startingelectronics.com
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
//#include <string>

#define SS_SD_CARD   4
#define SS_ETHERNET 10




// MAC address from Ethernet shield sticker under board
byte mac[] = { 0x00, 0x1D, 0xFE, 0xEA, 0x39, 0x17 };
IPAddress ip=(192, 168, 1, 26); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80

String HTTP_req;//     // stores the HTTP request
    char outBuf[200];
void setup()
{
  Serial.begin(9600);       // for diagnostics
  pinMode(SS_SD_CARD, OUTPUT);
  pinMode(SS_ETHERNET, OUTPUT);
  digitalWrite(SS_SD_CARD, HIGH); // disable SD card
  digitalWrite(SS_ETHERNET, LOW); // enable ethernet
  Serial.println("disabled sd card, enabled ethernet");
 
  getIP();

}

void getIP(){
    // start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.write("Failed to configure Ethernet using DHCP");
    // initialize the ethernet device not using DHCP:
    Ethernet.begin(mac, ip);
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print("."); 
  } 
  Serial.println("");
  
  
    server.begin();           // start to listen for clients
    Serial.println("listening...");
}

void loop()
{
    HTTP_req="";
    EthernetClient client = server.available();  // try to get client
    
    

//    sprintf(outBuf,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n<html><h1><HELLO></h1></html>");
//    client.write(outBuf);
/*
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("HELLO");*/
    
    if (client) {  // got client?
        Serial.println("connected to client");
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                HTTP_req+= c;  // save the HTTP request 1 char at a time
               
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    Serial.println("received request:\n:BEGIN:\n"+HTTP_req+":END:");
                    respondToRequest(client,HTTP_req);
                    //sprintf(outBuf,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n<html><h1><HELLO></h1></html>");
                    //client.write(outBuf);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
            else{
             client.stop(); 
            }
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

void respondToRequest(EthernetClient client, String & request){
  
  
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                //client.println("Refresh: 1");
                client.println();
                client.println("<!DOCTYPE html>");
                client.println("<html>");
                client.println("<head>");
                client.println("<title>Arduino LED Control</title>");
                client.println("</head>");
                client.println("<body>");
                client.println("<h1>HELLO</h1>");
  
  
  
}
