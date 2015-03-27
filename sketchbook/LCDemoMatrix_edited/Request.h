#ifndef SNOWDUINO_REQUEST_H_
#define SNOWDUINO_REQUEST_H_

#include <Ethernet.h>
#include "BannerCommand.h"


class Request{
  private:
    EthernetClient client_;
    char* request_buffer_;
  public:
    Request(EthernetClient client, char* buffer){
       client_ = client;
       request_buffer_ = buffer;
    }
    /*bool isFaviconRequest(){
      if(strstr(firstLine,"GET /favicon.ico HTTP/1.1"))return;
    }*/
    
    
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
    
    
    
    
    BannerCommand respond(){
      char* firstLine = strtok(request_buffer_,"\n"); // GET /?banner=hello&other=somethingelse HTTP/1.1
      if(strstr(firstLine,"GET /favicon.ico HTTP/1.1"))return null;
      
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
              return BannerCommand(param_value);
              /*banner = Banner(&canvas,param_value,strlen(param_value));
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/plain");
              client.println("Connection: close");
              client.println();
              client.println("Snowduino is now saying the following: ");
              client.println(param_value);*/
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
  
};

#endif
