/****** VARIABLES AND PIN ASSIGNMENTS *********************/

const byte indexInterrupt = 2;
const byte motorPin1 = 5;
const byte motorPin2 = 6;
const byte PWMpin = 7;
int indexCount = 0;
const int interval = 1000;
int PWMinterval = 50;
int currentMillis = 0;
int prevMillis = 0;
int PWMcurrentMillis = 0;
int PWMprevMillis = 0;
bool state = HIGH;
String string = "RPM = ";
int indexCountPrev = 0;
int currentRPM = 0;
String outputString;
int i = 0;

/****** SETUP *********************************************/

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(indexInterrupt, INPUT_PULLUP);
  pinMode(PWMpin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(indexInterrupt), interrupt, RISING);
  Serial.begin(9600);
}

/****** CUSTOM FUNCTIONS **********************************/
void motorForward()
{
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

void motorReverse()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

void motorStop()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

void interrupt()
{
  indexCount++;
}

/****** MAIN LOOP *****************************************/

void loop() {

  motorForward();

  currentMillis = millis();

  if(currentMillis - prevMillis >= interval)
  {
    
    currentRPM = (indexCount - indexCountPrev) * 60;
    outputString = string + currentRPM + "\n";
    Serial.print(outputString);
    prevMillis = currentMillis;
    
  }

  if(PWMcurrentMillis - PWMprevMillis >= PWMinterval)
  {
    state = !state;
    digitalWrite(PWMpin, state);
    
    if(i % 2 != 0)
    {
      PWMinterval = 50;
    } else {
      PWMinterval = 50;
    }
    i++;
  }

}
