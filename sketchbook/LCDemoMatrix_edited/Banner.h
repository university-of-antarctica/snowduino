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
    static const int CHAR_WIDTH = 5;
    static const int PADDING_BETWEEN_CHARS = 1;
    static const int PADDED_CHAR_WIDTH = CHAR_WIDTH+PADDING_BETWEEN_CHARS;
    int MAX_NUM_CHARS_ON_DISPLAY;//=4;// = 1+; // = 1+(16 - 1)/6 rounded up
    Canvas* canvas;
    
    // Helper function
    // Returns the smallest nonnegative integer x, 
    // such that  a+x = yb, where a,b,y are integers
    // myMod(n,5) from n=6 to n=-5 is ... 
    // { 1, 0, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0 }
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
      this->canvas = canvas;
      setPhrase(phrase);
      // calculate the most chars that could possibly be partially
      // visible at the same time
      int divisor = PADDED_CHAR_WIDTH;
      int dividend = canvas->num_cols() - 1;
      int rounded_up = ( dividend  + divisor - 1 ) / divisor;
      MAX_NUM_CHARS_ON_DISPLAY = 1 + rounded_up;
      
    }
    
    void setPhrase(const char* phrase){
      this->phrase = phrase;
      this->shAmt = 0;
      this->phraseLength = strlen(phrase);
      this->nCols = phraseLength*PADDED_CHAR_WIDTH;
    }
    
    void print(){
      int perCharacterScroll;
      Character currentChar;
      int indexOfFirstChar = -shAmt/PADDED_CHAR_WIDTH;
      int indexOfLastChar = indexOfFirstChar+MAX_NUM_CHARS_ON_DISPLAY;
      
      for(int i = indexOfFirstChar; i < indexOfLastChar; ++i){
        perCharacterScroll = shAmt+i*PADDED_CHAR_WIDTH;
        currentChar = Character(canvas, at(i));
        currentChar.printScrolledRightBy(perCharacterScroll);
      }
    }
    
    // Allows indexing into the phrase in a modular way.
    // indexing out of range from 0 to phraseLength-1 will index to
    // another part of the phrase.
    // Additionally, empty spaces are added between the repeated phrase
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
