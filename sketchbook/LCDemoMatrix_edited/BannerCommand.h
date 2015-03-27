#ifndef SNOWDUINO_BANNER_COMMAND_H_
#define SNOWDUINO_BANNER_COMMAND_H_
#include "Banner.h"

class BannerCommand{
  private:
  char* str_;
  public:
  BannerCommand(char* arg){
    str_ = arg;
  }
  void sendTo(Banner* b){
    b->setPhrase(str_);
  }
  
};


#endif
