#ifndef SNOWDUINO_BANNER_COMMAND_H_
#define SNOWDUINO_BANNER_COMMAND_H_
#include "Banner.h"
/*
class Command{
  public:
  virtual void sendTo(Banner* b);
  char description;
};

class NoCommand : public Command{
  public:
  void sendTo(Banner* b){
  }
  char description = 'n';
};

*/
class BannerCommand{
  private:
  char* param_value_;
  bool do_nothing_;
  public:
  BannerCommand(char* arg){
    param_value_ = arg;
    do_nothing_=false;
  }
  BannerCommand(){
    do_nothing_=true;
  }
  void sendTo(Banner* b){
    if(!do_nothing_)b->setPhrase(param_value_);
  }
  char* param_value(){
    return param_value_;
  }
  
};


#endif
