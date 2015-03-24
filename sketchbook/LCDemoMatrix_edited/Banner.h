#ifndef SNOWDUINO_BANNER_H_
#define SNOWDUINO_BANNER_H_

#include "Character.h"
#include "Canvas.h"

class Banner{
  private:
    const char* phrase;
    int phraseLength;
    int nCols;
    int shAmt;
    int clearance = 2;
    Canvas* canvas;
    
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
    
    public:
    Banner(){
    }
    Banner(Canvas* canvas, const char* phrase, int length){
      this->phrase = phrase;
      this->shAmt = 0;
      this->phraseLength = length;
      this->nCols = (phraseLength+1)*6;
      this->canvas = canvas;
      
      /*for(int i = 0; i < phraseLength; ++i){
        chars[i] = Character(canvas,phrase[i]);
      }*/
    }
    void print(){
      int perCharacterScroll;
      Character currentChar;
      int indexOfFirstChar = -shAmt/6; //shamt = -10: 1 //shamt = -12: 2 //shamt = 10 : 
      int indexOfLastChar = indexOfFirstChar+4;
      
      for(int i = indexOfFirstChar; i < indexOfLastChar; ++i){
        perCharacterScroll = shAmt+i*6;
        currentChar = Character(canvas, at(i));
        currentChar.printScrolledRightBy(perCharacterScroll);
        canvas->clearCol(perCharacterScroll-1);

      }
    }
    
    char at(int index){
      // Without the +clearance, the result of this mod would just convert increasing indices to rotating from 0,1,..,n-2,n-1,0,1,...
      // With the +clearance, it now is 0,1,...,n-1,n,,...n+clearance-1,0,1,...
      // This means we have a couple extra spots in the rotation, and we can make those return spaces instead of the actual characters in phrase
      int bannerLengthWithAddedClearance = phraseLength+clearance;
      int totalIndex = myMod(index,bannerLengthWithAddedClearance);
      if(totalIndex>=phraseLength)return ' ';
      return phrase[myMod(index,phraseLength+clearance)];
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
      /*if(shAmt < -nCols){
        shAmt = 32;
//        flipChars();
//        flipOrder();
      }
      if(shAmt > nCols) shAmt = -32;*/
    }
    
    void shiftLeft(int shAmt){
      shiftRight(-shAmt);
      
    }
    
};
#endif
