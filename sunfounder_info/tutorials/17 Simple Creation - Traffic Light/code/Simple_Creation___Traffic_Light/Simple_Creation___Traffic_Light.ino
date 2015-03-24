#define redPin  7   //red led attach to 
#define yellowPin 8   //yellow led attach to 
#define greenPin 9   //green led attach to 
#define buttonPin 6   //the number of the button

int state = 0;

void setup()
{
  pinMode(redPin, OUTPUT);  //set the redPin as an output
  pinMode(yellowPin, OUTPUT);  //set the yellowPin as an output
  pinMode(greenPin, OUTPUT);  //set the greenPin as an output
  pinMode(buttonPin, INPUT);  //set the buttonPin as an input
  Serial.begin(9600);  // start serial port at 9600 bps:
}

void loop()
{
  while(digitalRead(buttonPin))
  {
    if(state == 0)  //when state is 0
    {
      setLights(HIGH, LOW, LOW); 
      state = 1;  // set state 1
    }
    else if(state == 1)
    {
      setLights(HIGH, HIGH, LOW);
      state = 2;  // set state 2
    }
    else if(state == 2)
    {
      setLights(LOW, LOW, HIGH);
      state = 3;  //set state 3
    }
    else if(state == 3)
    {
      setLights(LOW, HIGH, LOW);
      state = 0;  //set state 0
    }
    delay(1000);
  }
}
// the function to set the led with the specified state(on or off),HIGH is on, and LOW is off
void setLights(int redState, int yellowState, int greenState)
{
  digitalWrite(redPin, redState);
  digitalWrite(yellowPin, yellowState);
  digitalWrite(greenPin, greenState);
}
