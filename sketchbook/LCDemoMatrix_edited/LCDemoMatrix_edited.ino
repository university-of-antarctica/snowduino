#include "LedControlMS.h"
#include <MemoryFree.h>
#include "Banner.h"
#include "Request.h"
#include "BannerCommand.h"
#include <SPI.h>
#include <Ethernet.h>
#include "NetworkSetup.h"






/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 7 is connected to the DataIn 
 pin 6 is connected to the CLK 
 pin 5 is connected to Load. 
 2 is the # of dispays
 */
const int DIN_PIN = 7;
const int CLK_PIN = 6;
const int CS_PIN  = 5;
const int NUM_DISPLAYS = 2;
Canvas canvas = Canvas(DIN_PIN,CLK_PIN,CS_PIN,NUM_DISPLAYS);




int counter = 0;
int iterations_since_last;
int requestLength = 0;

Banner banner;
NetworkSetup network;


//char requestBuffer[MAX_CHARACTERS] = "";
/*
void out(char* str){
  Serial.print(F(str));
}
void outln(char* str){
  Serial.println(F(str));
}*/



void setup() {
  Serial.begin(9600);
  Serial.println(F("Serial starting..."));
  Serial.print(F("free memory: "));
  Serial.println(freeMemory());
  
  
  network = NetworkSetup();
  network.Setup();
  Serial.println(F("listening..."));
  
  banner = Banner(&canvas,"hello world!",15);
}
void loop() { 
  // Periodically print out the remaining free memory
  if(!(counter%100)){
    Serial.print(F("free memory: "));
    Serial.println(freeMemory());
  }
  
  if(!(counter%5000)){
    banner.setPhrase("Welcome! go to http://uofantarctica.com to change me!");
  }
  counter++;
  
  writeArduinoOnMatrix();
  network.beServer();
  
}

void processBannerCommand(BannerCommand* cmd ){
  cmd->sendTo(&banner);
  delete cmd;
}




void writeArduinoOnMatrix() {
  canvas.clearDisplay();
  banner.print();
  banner.shiftLeft(1);  
  delay(4);
}
