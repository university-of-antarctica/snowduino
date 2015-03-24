#ifndef SNOWDUINO_BANNER_H_
#define SNOWDUINO_BANNER_H_

#include "Character.h"
#include "Canvas.h"

class Banner{
  private:
    String phrase;
    int phraseLength;
    int nCols;
    int shAmt;
    //Character chars[10];
    Canvas* canvas;
    
    public:
    Banner(){
    }
    Banner(Canvas* canvas, String phrase){
      this->phrase = phrase;
      this->shAmt = 0;
      this->phraseLength = phrase.length();
      this->nCols = (phraseLength+1)*6;
      this->canvas = canvas;
      
      /*for(int i = 0; i < phraseLength; ++i){
        chars[i] = Character(canvas,phrase[i]);
      }*/
    }
    void print(){
      int perCharacterScroll;
      Character currentChar;
      for(int i = 0; i < phraseLength; ++i){
        perCharacterScroll = shAmt+i*6;
        currentChar = Character(canvas, phrase[i]);
        currentChar.printScrolledRightBy(perCharacterScroll);
        canvas->clearCol(perCharacterScroll-1);

      }
    }
    
    /*void flipChars(){
      for(int i = 0; i < phraseLength; ++i){
        chars[i].flipVertical();
        chars[i].flipHorizontal();
      }
    }*/
    
    /*void flipOrder(){
      int j = phraseLength;
      Character temp;
      for(int i = 0; i < phraseLength/2; ++i){
        j = phraseLength-1-i;
        temp = chars[i];
        chars[i]=chars[j];
        chars[j]=temp;
      }
    }*/
    
    void shiftRight(int shiftBy){
      this->shAmt+=shiftBy;
      //Serial.println(shAmt);
      if(shAmt < -nCols){
        shAmt = 16;
//        flipChars();
//        flipOrder();
      }
      if(shAmt > 16) shAmt=-nCols;
    }
    
    void shiftLeft(int shAmt){
      shiftRight(-shAmt);
      
    }
    
};
#endif
