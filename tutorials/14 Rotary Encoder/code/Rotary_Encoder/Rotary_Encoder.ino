#define clkPin 2
#define dtPin 3
#define swPin 4 //the number of the button

int encoderVal = 0;

void setup()
{
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);
  Serial.begin(9600);
}

void loop()
{
  int change = getEncoderTurn();
  encoderVal = encoderVal + change;
  if(digitalRead(swPin) == LOW)
  {
    encoderVal = 0;
  }
  Serial.println(encoderVal);
}

int getEncoderTurn(void)
{
  static int oldA = HIGH;
  static int oldB = HIGH;
  int result = 0;
  int newA = digitalRead(clkPin);
  int newB = digitalRead(dtPin);
  if (newA != oldA || newB != oldB)
  {
    // something has changed
    if (oldA == HIGH && newA == LOW)
    {
      result = (oldB * 2 - 1);
    }
  }
  oldA = newA;
  oldB = newB;
  return result;
}
