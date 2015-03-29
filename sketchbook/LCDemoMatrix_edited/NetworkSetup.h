#ifndef SNOWDUINO_NETWORKSETUP_H_
#define SNOWDUINO_NETWORKSETUP_H_

#define SS_SD_CARD   4
#define SS_ETHERNET 10

#include "Request.h"


class NetworkSetup{
  private:
  
    static const int PORT = 8282;
    static const int MAX_CHARACTERS = 200;
    

    EthernetServer* server_;//(PORT);
    EthernetClient client_;
    Request last_request_;
    bool unread_request_;
    
    char requestBuffer[MAX_CHARACTERS];
    
    int request_length_;
    boolean keep_recording_;
    boolean current_line_is_blank_;
    
    
    byte mac[6] = { 0x00, 0x1D, 0xFE, 0xEA, 0x39, 0x17 };
    IPAddress ip=(192, 168, 1, 19); // IP address, may need to change depending on network

    
    void EnableEthernet(){
      pinMode(SS_SD_CARD, OUTPUT);
      pinMode(SS_ETHERNET, OUTPUT);
      digitalWrite(SS_SD_CARD, HIGH); // disable SD card
      digitalWrite(SS_ETHERNET, LOW); // enable ethernet
      Serial.println("disabled sd card, enabled ethernet");
    }
    
    void JoinNetwork(){
      Serial.println("Trying to get an IP address using DHCP");
      if (Ethernet.begin(mac) == 0) {
        Serial.write("Failed to configure Ethernet using DHCP");
        // initialize the ethernet device not using DHCP:
        Ethernet.begin(mac, ip);
      } 
    }
    void PrintIp(){
      // print your local IP address:
      Serial.print(F("My IP address: "));
      ip = Ethernet.localIP();
      for (byte thisByte = 0; thisByte < 4; thisByte++) {
        // print the value of each byte of the IP address:
        Serial.print(ip[thisByte], DEC);
        Serial.print("."); 
      } 
      Serial.println("");
    }
  
  
    void ProcessCharFromClient(char c){
      if(keep_recording_){
        requestBuffer[request_length_] = c;  // save the HTTP request 1 char at a time
        ++request_length_;
        if(request_length_ > MAX_CHARACTERS){
          keep_recording_ = false;
        }
      }
     
      // last line of client request is blank and ends with \n
      // respond to client only after last line received
      if (c == '\n' && current_line_is_blank_) {
          requestBuffer[request_length_]='\0';
          Serial.print(F("received request of length "));
          Serial.print(request_length_);
          Serial.print(F(":\n:BEGIN:\n"));
          Serial.print(requestBuffer);
          Serial.println(F(":END:"));
          
          //strcpy(requestBuffer,bannerBuffer);
          
          last_request_ = Request(requestBuffer);
          return;
      }
      // every line of text received from the client ends with \r\n
      if (c == '\n') {
          // last character on line of received text
          // starting new line with next character read
          keep_recording_ = false;
          current_line_is_blank_ = true;
      } 
      else if (c != '\r') {
          // a text character was received from client
          current_line_is_blank_ = false;
      }
    }
  
    
  public:
  
    
    void beServer(){
      bool keepRecording;
      EthernetClient client = server_->available();  // try to get client
      if (client) {  // got client?
        client_ = client;
        requestLength=0;
        keepRecording=true;
          Serial.println("connected to client");
          boolean currentLineIsBlank = true;
          while (client.connected()) {
              if (client.available()) {   // client data available to read
                  char c = client.read(); // read 1 byte (character) from client
                  ProcessCharFromClient(c);
              } // end if (client.available())
              else{
               client.stop(); 
              }
          } // end while (client.connected())
          delay(1);      // give the web browser time to receive the data
          client.stop(); // close the connection
      } // end if (client)
    }
    
    Request readLastRequest(){
      return last_request_;
    }

    void Setup(){
      EnableEthernet();
      JoinNetwork();
      PrintIp();
      server_ = new EthernetServer(PORT);
      server_->begin();           // start to listen for clients
    }

    
};
#endif
