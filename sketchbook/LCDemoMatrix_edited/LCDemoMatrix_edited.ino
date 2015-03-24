//We always have to include the library


#include "LedControlMS.h"
#include <MemoryFree.h>
//#include "font_5x7.h"
#include "Canvas.h"
//#include <avr/pgmspace.h>
#include "Character.h"






/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have two MAX72XXs. 
 */
LedControl* lc=new LedControl(12,11,10,2); // LAST ARGUMENT IS NUMBER OF LED ARRAYS - STEVEN
Canvas canvas = Canvas(lc);

/* we always wait a bit between updates of the display */
unsigned long delaytime=5;


















class Banner{
  private:
    String phrase;
    int phraseLength;
    int nCols;
    int shAmt;
    Character chars[64];
    
    public:
    Banner(){
    }
    Banner(String phrase){
      this->phrase = phrase;
      this->shAmt = 0;
      this->phraseLength = phrase.length();
      this->nCols = (phraseLength+1)*6;
      
      for(int i = 0; i < phraseLength; ++i){
        chars[i] = Character(&canvas,phrase[i]);
      }
    }
    void print(){
      int perCharacterScroll;
      for(int i = 0; i < phraseLength; ++i){
        perCharacterScroll = shAmt+i*6;
        chars[i].printScrolledRightBy(perCharacterScroll);
      }
    }
    
    void flipChars(){
      for(int i = 0; i < phraseLength; ++i){
        chars[i].flipVertical();
        chars[i].flipHorizontal();
      }
    }
    
    void flipOrder(){
      int j = phraseLength;
      Character temp;
      for(int i = 0; i < phraseLength/2; ++i){
        j = phraseLength-1-i;
        temp = chars[i];
        chars[i]=chars[j];
        chars[j]=temp;
      }
    }
    
    void shiftRight(int shiftBy){
      this->shAmt+=shiftBy;
      //Serial.println(shAmt);
      if(shAmt < -nCols){
        shAmt = 16;
        flipChars();
        flipOrder();
      }
      if(shAmt > 16) shAmt=-nCols;
    }
    void shiftLeft(int shAmt){
      shiftRight(-shAmt);
      
    }
    
};












int spacing = 0;
Banner banner;







void writeArduinoOnMatrix() {
/*
  lc.setRow(0,0,0xFF);
  lc.setRow(1,1,0xF);*/
  canvas.clearDisplay();
  
  Character A = Character(&canvas,'A');
  /*Character r = Character('r');
  Character d = Character('d');
  Character u = Character('u');
  Character i = Character('i');
  Character n = Character('n');
  Character o = Character('o');
  
  A.printScrolledRightBy(spacing);
  r.printScrolledRightBy(spacing+6);
  d.printScrolledRightBy(spacing+12);
  u.printScrolledRightBy(spacing+18);
  i.printScrolledRightBy(spacing+24);
  n.printScrolledRightBy(spacing+30);
  o.printScrolledRightBy(spacing+36);
  */
  //A.printScrolledRightBy(1);
  if(!(spacing%100)){
    Serial.print("free memory: ");
    Serial.println(freeMemory());
  }
  
  banner.print();
  banner.shiftLeft(1);
  
  
  delay(delaytime);
  spacing++;
  
}




void setup() {
  
  Serial.begin(9600);
  
  

  
  /* Set the max # of rows of the 0th display to all 8 */
//  lc.setScanLimit(0,8);
  /* Set the max # of rows of the 1st display to all 8 */
//  lc.setScanLimit(1,8);
  
  Serial.println("Serial starting...");
    Serial.print("free memory: ");
  Serial.println(freeMemory());
 // Serial.println(lc.getDeviceCount());
  
  banner = Banner("Yo, one time, I sucked six in a row!");
  /* and clear the display */
  //lc.clearDisplay(0);
}
void loop() { 
  writeArduinoOnMatrix();
  //rows();
  //columns();
  //single();
}
