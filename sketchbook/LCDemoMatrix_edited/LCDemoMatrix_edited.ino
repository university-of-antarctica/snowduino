#include "LedControlMS.h"
#include <MemoryFree.h>
#include "Banner.h"
#include "NetworkSetup.h"
#include <SPI.h>
#include <Ethernet.h>
#include "Request.h"
#include "BannerCommand.h"
#define SS_SD_CARD   4
#define SS_ETHERNET 10

// MAC address from Ethernet shield sticker under board


/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 7 is connected to the DataIn 
 pin 6 is connected to the CLK 
 pin 5 is connected to Load. 
 2 is the # of dispays
 */
const int DIN_PIN = 7;
const int CLK_PIN = 6;
const int CS_PIN  = 5;
const int NUM_DISPLAYS = 2;
Canvas canvas = Canvas(DIN_PIN,CLK_PIN,CS_PIN,NUM_DISPLAYS);
EthernetServer server(8282);

int counter = 0;
Banner banner;
char bannerBuffer[512] = "";
char requestBuffer[512] = "";
int requestLength = 0;
EthernetClient client_;


void setup() {
  Serial.begin(9600);
  Serial.println("Serial starting...");
  Serial.print("free memory: ");
  Serial.println(freeMemory());
  
  
  NetworkSetup ns = NetworkSetup();
  ns.Setup();
  server.begin();           // start to listen for clients
  Serial.println("listening...");
  
  banner = Banner(&canvas,"hello",6);
}
void loop() { 
  // Periodically print out the remaining free memory
  if(!(counter%100)){
    Serial.print("free memory: ");
    Serial.println(freeMemory());
  }
  counter++;
  
  writeArduinoOnMatrix();
  beServer();
  
}

void processBannerCommand(Command* cmd ){
  client_.println("HTTP/1.1 200 OK");
  client_.println("Content-Type: text/plain");
  client_.println("Connection: close");
  client_.println();
  //client_.println("Snowduino is now saying the following: ");
  //client_.println(cmd->param_value());
  //banner.setPhrase(cmd.param_value());
  cmd->sendTo(&banner);
  Serial.print("command description: ");
  Serial.println(cmd->description);
  delete cmd;
}


void beServer(){
  bool keepRecording;
    EthernetClient client = server.available();  // try to get client
    if (client) {  // got client?
      client_ = client;
      requestLength=0;
      keepRecording=true;
        Serial.println("connected to client");
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                if(keepRecording){
                  requestBuffer[requestLength] = c;  // save the HTTP request 1 char at a time
                  ++requestLength;
                }
               
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    requestBuffer[requestLength]='\0';
                    Serial.print("received request of length ");
                    Serial.print(requestLength);
                    Serial.print(":\n:BEGIN:\n");
                    Serial.print(requestBuffer);
                    Serial.println(":END:");
                    Request r = Request(client,requestBuffer);
                    Command* cmd = r.respond();
                    processBannerCommand(cmd);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    keepRecording = false;
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


void writeArduinoOnMatrix() {
  canvas.clearDisplay();
  
  banner.print();
  banner.shiftLeft(1);  
}
