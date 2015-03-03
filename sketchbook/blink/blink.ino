

// constants
const int redLed = 2;
const int buttonPin = 5;

// variables that change
int buttonState = 0;
int blinkDelay = 1000;


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(redLed, OUTPUT);  
  // read from the button pin
  pinMode(buttonPin, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    blinkDelay=100;
  }
  else{
    blinkDelay=1000;
  }  
  digitalWrite(redLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(blinkDelay);
  digitalWrite(redLed, LOW);    // turn the LED off by making the voltage LOW
  delay(blinkDelay);
}
