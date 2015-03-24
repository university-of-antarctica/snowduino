#ifndef SNOWDUINO_CANVAS_H_
#define SNOWDUINO_CANVAS_H_



class Canvas{
  private:
    LedControl* lc;
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
    Canvas(LedControl* lc){
      this->lc = lc;
        /*
         The MAX72XX is in power-saving mode on startup,
         we have to do a wakeup call
         */
        lc->shutdown(0,false);
        lc->shutdown(1,false);
        
        /* Set the brightness to a low value */
        lc->setIntensity(0,1);
        lc->setIntensity(1,1);
    }
    void clearCol(int colNum){
      mySetCol(colNum,B00000000);
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
      lc->setRow(actualDevice,actualRow,bits);
    }
    void clearDisplay(){
        lc->clearDisplay(0);
        lc->clearDisplay(1);
    }
};
#endif
