/**************************************/
const int lowestPin = 2;//the lowest one attach to
const int highestPin = 9;//the highest one attach to 
/**************************************/
void setup()
{
  //set pins 1 through 6 as output  
  for(int thisPin = lowestPin;thisPin <= highestPin;thisPin++)
  {
     pinMode(thisPin,OUTPUT); //initialize thisPin as an output
  }
} 
/****************************************/
void loop()
{
  //iterate over the pins
  //turn the led on from lowest to the highest
  for(int thisPin = lowestPin;thisPin <= highestPin;thisPin++)
  {
    digitalWrite(thisPin,HIGH);//turn this led on
    delay(100);//wait for 100 microseconds
  }
  //fade from the highest to the lowest
  for(int thisPin = highestPin;thisPin>=lowestPin;thisPin--)
  {
    digitalWrite(thisPin,LOW);//turn this led off
    delay(100);//wait for 100 microseconds
  }
  for(int thisPin = highestPin;thisPin>=lowestPin;thisPin--)
  {
    digitalWrite(thisPin,HIGH);//turn this led off
    delay(100);//wait for 100 microseconds
  }
  for(int thisPin = lowestPin;thisPin <= highestPin;thisPin++)
  {
    digitalWrite(thisPin,LOW);//turn this led on
    delay(100);//wait for 100 microseconds
  }
}
