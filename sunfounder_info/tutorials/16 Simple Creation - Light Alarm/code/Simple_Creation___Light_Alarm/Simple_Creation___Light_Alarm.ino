void setup()
{
  Serial.begin(9600);  // start serial port at 9600 bps:
}
void loop()
{
  int n=analogRead(A0);   //read the value from analog pin AO  
  if(n>0)                 //If there is a voltage
  {
    pinMode(4,OUTPUT);         //set the digital pin 4 as an output
    tone(4,10000);          //Generates a square wave of the frequency of 10000 Hz (and 50% duty cycle) on a pin 4 
    pinMode(4,INPUT);           //set the pin 4 as an input
  }
}

