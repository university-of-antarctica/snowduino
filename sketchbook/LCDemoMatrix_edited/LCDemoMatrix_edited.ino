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


void beServer(){
  
  //parsing the request one character at a time... how to proceed?
  // allocate a large block (512 bytes)
  // a short request is around 300 bytes
  // the get parameters are on the first line
  // read the first line until we get to the ? after the url path
  // read until the space between the get parameters and "HTTP/1.1"
  // we want to convert %20 to ' ' 
  
  
  
  
  
  
  
  bool keepRecording;
    EthernetClient client = server.available();  // try to get client
    if (client) {  // got client?
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
                    BannerCommand cmd = r.respond();
                    cmd.sendTo(&banner);
                    //respondToRequest(client,requestBuffer);
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


// Iterate over a string and remove the % url-encodings
void removePercentTwenties(char* httpString){
  int len = strlen(httpString);
  char c;
  int code;
  char* percent_loc;
  char* after_percent;
  int after_percent_index;
  // iterate over entire string searching for % codes to eliminate
  for(int i = 0; i < len; ++i){
    c = *(httpString+i);
    if(c=='%'){  
      percent_loc = httpString+i;
      after_percent = httpString+i+1;
      after_percent_index = i+1;
      code = strtol(after_percent,0, 16);
      // replace the percent with the character that the code represents
      *(percent_loc)=code;
      // shift the remainder of the string to the left by 2 after the percent
      for(int j = after_percent_index; j < len; j++){
        *(httpString+j) = *(httpString+j+2); 
      }
    } 
  }  
}


/*
void parseBannerCmd(char* param_value){
  Serial.print("banner command activated!: ");
  Serial.println(param_value);
  banner = Banner(&canvas,param_value,strlen(param_value));
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println("Snowduino is now saying the following: ");
  client.println(param_value);
}

void parseArgPair(char* param_name, char* param_value){
  if(strcmp(param_name,"banner") == 0){
     parseBannerCmd(param_value);
  }
  else{
     
  }
}*/

void respondToRequest(EthernetClient client, char* request){
  
                char* firstLine = strtok(request,"\n"); // GET /?banner=hello&other=somethingelse HTTP/1.1
                if(strstr(firstLine,"GET /favicon.ico HTTP/1.1"))return;
                
                strtok(firstLine,"/ ?");                // GET
                char* params = strtok(0,"/ ?");         //       banner=hello&other=somethingelse
                char* token = strtok(params,"=");       //       banner
                
                char* param_name;
                char* param_value;
                
                while( token != '\0' ){
                     param_name = token;
                     param_value = strtok(0,"& \n");
                     removePercentTwenties(param_value);
                     
                     if(strcmp(param_name,"banner") == 0){
                        Serial.print("banner command activated!: ");
                        Serial.println(param_value);
                        banner = Banner(&canvas,param_value,strlen(param_value));
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/plain");
                        client.println("Connection: close");
                        client.println();
                        client.println("Snowduino is now saying the following: ");
                        client.println(param_value);
                     }
                     else{
                        Serial.print(param_name);
                        Serial.print(" (unknown command) : ");
                        Serial.println(param_value); 
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/plain");
                        client.println("Connection: close");
                        client.println();
                        client.println("commands: ");
                        client.println(param_name);
                        client.println(param_value);
                     }
                     
                    token = strtok(0,"= \n");
                }          
}


void writeArduinoOnMatrix() {
  canvas.clearDisplay();
  
  banner.print();
  banner.shiftLeft(1);  
}
