#include "LedControlMS.h"
#include <MemoryFree.h>
#include "Banner.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 2 is the nu
 */
const int DIN_PIN = 12;
const int CLK_PIN = 11;
const int CS_PIN  = 10;
const int NUM_DISPLAYS = 2;
 
 
LedControl* lc=new LedControl(DIN_PIN,CLK_PIN,CS_PIN,NUM_DISPLAYS); 
Canvas canvas = Canvas(lc);
int counter = 0;
Banner banner;



void setup() {
  Serial.begin(9600);
  Serial.println("Serial starting...");
  Serial.print("free memory: ");
  Serial.println(freeMemory());
  
  banner = Banner(&canvas,"Yo, one time, I sucked six in a row!");
}
void loop() { 
  // Periodically print out the remaining free memory
  if(!(counter%100)){
    Serial.print("free memory: ");
    Serial.println(freeMemory());
  }
  counter++;
  
  writeArduinoOnMatrix();
  
}

void writeArduinoOnMatrix() {
  canvas.clearDisplay();
  
  banner.print();
  banner.shiftLeft(1);  
  //delay(delaytime);
}
