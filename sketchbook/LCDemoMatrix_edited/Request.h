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
    
    
    Command* parseArgPair(char* param_name, char* param_value){
      if(strcmp(param_name,"banner") == 0){
        Serial.print("parseArgPair: ");
        Serial.println(param_value);
        return new BannerCommand(param_value);
      }
      else{
        return new NoCommand();
      }
    }
    
    Command* respond(){
      char* firstLine = strtok(request_buffer_,"\n"); // GET /?banner=hello&other=somethingelse HTTP/1.1
      if(strstr(firstLine,"GET /favicon.ico HTTP/1.1"))return new NoCommand();
      
      strtok(firstLine,"/ ?");                // GET
      char* params = strtok(0,"/ ?");         //       banner=hello&other=somethingelse
      char* token = strtok(params,"=");       //       banner
      
      char* param_name;
      char* param_value;
      
      while( token != '\0' ){
         param_name = token;
         param_value = strtok(0,"& \n");
         removePercentTwenties(param_value);
         return parseArgPair(param_name,param_value);
         token = strtok(0,"= \n");
      }      
    }
  
};

#endif
