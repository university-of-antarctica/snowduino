#ifndef SNOWDUINO_CANVAS_H_
#define SNOWDUINO_CANVAS_H_
#include "LedControlMS.h"

// The Canvas class wraps around the LedControl class 
// and duplicates its functionality in such a way that 
// simulates a larger single display instead of multiple different displays.
class Canvas{
  private:
    LedControl* lc_;
    int device_count_;
  public:
    Canvas(LedControl* lc){
      this->lc_ = lc;
      this->device_count_ = lc_->getDeviceCount();
      // The MAX72XX starts off in power-saving mode
      // we have to do a wakeup call
      shutdown(false);
      // Lower the brightness for my eyes
      setIntensity(1);
    }
    
    Canvas(const int DIN, const int CLK, const int CS, const int num_devices){
      this->lc_ = new LedControl(DIN,CLK,CS,num_devices);
      this->device_count_ = num_devices;
      shutdown(false);
      setIntensity(1);
    }
    
    void shutdown(bool turn_off){
      for(int i = 0; i < device_count_; ++i){
        lc_->shutdown(i,turn_off);
      }
    }
    
    void setIntensity(int level){
      for(int i = 0; i < device_count_; ++i){
        lc_->setIntensity(i,level);
      }
    }

    void setCol(int col, byte bits){
      if(col<0)return;
      int actualDevice = col/8;
      int deviceRow = col%8;
      lc_->setRow(actualDevice,deviceRow,bits);
    }
    
    // setRow(int row, byte bits) is not implemented for two reasons
    // it would map to the inefficient setColumn method in LedControl
    // it would consist of more than 8 dots, and need to take a pointer
    // to a character array instead of a byte
    
    void clearCol(int col){
      setCol(col,B00000000);
    }
    
    void clearDisplay(){
      clearAll();
    }
    void clearAll(){
      lc_->clearAll();
    }
    
    // untested
    void setLed(int row, int col, boolean state){
      if(col<0)return;
      int actualDevice = col/8;
      int deviceRow=col%8;
      int deviceCol=row;
      lc_->setLed(actualDevice,deviceRow,deviceCol,state);
    }
    
    int num_cols(){
      return device_count_*8;
    }
    
};
#endif
