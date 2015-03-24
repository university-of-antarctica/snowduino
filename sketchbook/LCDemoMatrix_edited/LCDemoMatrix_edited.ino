//We always have to include the library
#include "LedControlMS.h"

#include <avr/pgmspace.h>

#include "font_5x7.h"





/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have two MAX72XXs. 
 */
LedControl lc=LedControl(12,11,10,2); // LAST ARGUMENT IS NUMBER OF LED ARRAYS - STEVEN

/* we always wait a bit between updates of the display */
unsigned long delaytime=20;


// Character class holds 5x7 characters
class Character{
  byte cols[5];
  public:
  Character(){}
  Character(byte r1, byte r2, byte r3, byte r4, byte r5){
    set(r1,r2,r3,r4,r5);
  }
  
  Character(char letter){
    set(letter);
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
    mySetCol(startCol,cols[0]);
    //delay(thedelay);
    mySetCol(startCol+1,cols[1]);
    //delay(thedelay);
    mySetCol(startCol+2,cols[2]);
    //delay(thedelay);
    mySetCol(startCol+3,cols[3]);
    //delay(thedelay);
    mySetCol(startCol+4,cols[4]);
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





class Banner{
  private:
    String phrase;
    int phraseLength;
    int nCols;
    int shAmt;
    Character chars[128];
    
    public:
    Banner(){
    }
    Banner(String phrase){
      this->phrase = phrase;
      this->shAmt = 0;
      this->phraseLength = phrase.length();
      this->nCols = (phraseLength+1)*6;
      
      for(int i = 0; i < phraseLength; ++i){
        chars[i] = Character(phrase[i]);
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


byte reverse(byte b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

// Returns the smallest nonnegative integer x, such that  a+x = yb, where a,b,y are integers
// Example: n % 5 from n = 6 -> -5 =  { 1, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0 }
// But myMod(n,5) from n = 6 -> -5 =  { 1, 0, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0 }
int myMod(int a, int b){
  if(b<0)b=-b; //b = abs(b)
  while(a < 0 ){
    a+=b;
  }
  return a%b;
}


void writeArduinoOnMatrix() {
/*
  lc.setRow(0,0,0xFF);
  lc.setRow(1,1,0xF);*/
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  /*
  Character A = Character('A');
  Character r = Character('r');
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
  banner.print();
  banner.shiftLeft(1);
  
  //delay(delaytime);
  spacing--;
  
}

void mySetCol(int colNum, byte bits){
  if(colNum<0)return;
  int actualDevice = colNum/8; //integer division
  //Serial.print("actualDevice ");
  //Serial.println(actualDevice);
  //if(actualDevice<0 || actualDevice >1) return;  // Don't waste time doing nothing
  int actualRow = myMod(colNum,8);
  //Serial.print("actual Row ");
  //Serial.println(actualRow);
  lc.setRow(actualDevice,actualRow,bits);
}


void setup() {
  
  Serial.begin(9600);
  
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  
  /* Set the brightness to a low value */
  lc.setIntensity(0,5);
  lc.setIntensity(1,5);
  
  /* Set the max # of rows of the 0th display to all 8 */
//  lc.setScanLimit(0,8);
  /* Set the max # of rows of the 1st display to all 8 */
//  lc.setScanLimit(1,8);
  
  Serial.println("Serial starting...");
  Serial.println(lc.getDeviceCount());
  
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
