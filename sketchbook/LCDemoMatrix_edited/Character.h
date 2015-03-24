#ifndef SNOWDUINO_CHARACTER_H_
#define SNOWDUINO_CHARACTER_H_

#include "font_5x7.h"
#include "Canvas.h"
#include <avr/pgmspace.h>

// Character class holds 5x7 characters
class Character{
  private:
    byte cols[5];
    Canvas* canvas;
    byte reverse(byte b) {
       b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
       b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
       b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
       return b;
    }
  public:
    Character(){//Canvas* c){
     // this->canvas = c;
    }
//    Character(byte r1, byte r2, byte r3, byte r4, byte r5){
//      set(r1,r2,r3,r4,r5);
//    }
    
    Character(Canvas* c, char letter){
      set(letter);
      this->canvas = c;
      flipVertical();
    }
    
    void set(char c){
      char myByte;
      byte bytes[5];
      for (int i = 0; i < 5; ++i){
        myByte =  pgm_read_byte_near(font_5x7[c-32] + i);
        bytes[i] = myByte;
      }
      set(bytes[0],bytes[1],bytes[2],bytes[3],bytes[4]);
    }
    
    
    void set(byte r1, byte r2, byte r3, byte r4, byte r5){
      cols[0]=r1;
      cols[1]=r2;
      cols[2]=r3;
      cols[3]=r4;
      cols[4]=r5;
    }
    
    void printScrolledRightBy(int scrollAmt){
      int startCol = scrollAmt;
      //if(scrollAmt < -4 || scrollAmt > 15) return;  // Don't waste time 
      //int thedelay = 0;
      //delay(thedelay);
     // Serial.print("start col: ");
      //Serial.println(startCol);//#include <avr/pgmspace.h>
      
      for( int i = 0; i < 5; ++i){
        //canvas->clearCol(startCol+i);
        canvas->setCol(startCol+i,cols[i]);
      }
      
      /*
      canvas->mySetCol(startCol,cols[0]);
      //delay(thedelay);
      canvas->mySetCol(startCol+1,cols[1]);
      //delay(thedelay);
      canvas->mySetCol(startCol+2,cols[2]);
      //delay(thedelay);
      canvas->mySetCol(startCol+3,cols[3]);
      //delay(thedelay);
      canvas->mySetCol(startCol+4,cols[4]);*/
    }
    
    void flipVertical(){
      for(int i = 0; i < 5; ++i){
        cols[i] = reverse(cols[i]); 
      }
    }
    
    void flipHorizontal(){
      byte temp;
      int j;
      for(int i = 0; i < 5/2; ++i){
        j = 5-1-i;  // Other end of the array 
        temp = cols[i];
        cols[i] = cols[j];
        cols[j] = temp;
      }
    }  
    
};


#endif
