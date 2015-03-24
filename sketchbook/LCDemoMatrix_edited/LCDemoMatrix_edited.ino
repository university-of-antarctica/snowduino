#include "LedControlMS.h"
#include <MemoryFree.h>
#include "Banner.h"

#include <SPI.h>
#include <Ethernet.h>
#define SS_SD_CARD   4
#define SS_ETHERNET 10
//String HTTP_req;//     // stores the HTTP request
// MAC address from Ethernet shield sticker under board
byte mac[] = { 0x00, 0x1D, 0xFE, 0xEA, 0x39, 0x17 };
IPAddress ip=(192, 168, 1, 19); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80



/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 2 is the nu
 */
const int DIN_PIN = 7;
const int CLK_PIN = 6;
const int CS_PIN  = 5;
const int NUM_DISPLAYS = 2;
Canvas canvas = Canvas(DIN_PIN,CLK_PIN,CS_PIN,NUM_DISPLAYS);


int counter = 0;
Banner banner;
char bannerBuffer[512] = "";
char requestBuffer[512] = "";
int requestLength = 0;



void ethernet_setup(){
  pinMode(SS_SD_CARD, OUTPUT);
  pinMode(SS_ETHERNET, OUTPUT);
  digitalWrite(SS_SD_CARD, HIGH); // disable SD card
  digitalWrite(SS_ETHERNET, LOW); // enable ethernet
  Serial.println("disabled sd card, enabled ethernet");
  
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


void setup() {
  Serial.begin(9600);
  Serial.println("Serial starting...");
  Serial.print("free memory: ");
  Serial.println(freeMemory());
  ethernet_setup();
  
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
                if(keepRecording)requestBuffer[requestLength] = c;  // save the HTTP request 1 char at a time
                ++requestLength;
               
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    requestBuffer[requestLength]='\0';
                    Serial.print("received request of length ");
                    Serial.print(requestLength);
                    Serial.print(":\n:BEGIN:\n");
                    Serial.print(requestBuffer);
                    Serial.println(":END:");
                    respondToRequest(client,requestBuffer);
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


void respondToRequest(EthernetClient client, char* request){
                  
                // Send a response
                /*client.println("HTTP/1.1 200 OK");
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
                client.println("<h1>HELLO</h1>");*/
                
                
                char* firstLine = strtok(request,"\n");
                strtok(firstLine," ");              // GET
                char* params = strtok(0," /?");     // /?banner=hello&other=somethingelse
               // params+=2;                        //   banner=hello&other=somethingelse 
                char* args = strtok(params,"=&");   //   banner,hello,other,somethingelse
                
                Serial.println(args);
                args = strtok(0,"=&"); // first thing after an =
                Serial.println(args);
                
                banner = Banner(&canvas,args,strlen(args));
}


void writeArduinoOnMatrix() {
  canvas.clearDisplay();
  
  banner.print();
  banner.shiftLeft(1);  
}
