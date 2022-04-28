#include <Stepper.h>
#include <Servo.h>

#define LONG 0
#define SHORT 1
#define BLACK 0
#define WHITE 1
#define UNKNOWN_POS -1

const int stepsPerRevolution = 2048;
// Wiring:
// Pin 10 to IN1, Pin 11 to IN2, Pin 12 to IN3, Pin 13 to IN4
int StepperPins[2][4] = {{6, 7, 8, 9}, {10, 11, 12 , 13}};

const int endEffectPin = 3;

const int limitSwitchPins[] = {5, 22};

const long int axisRangeMaxSteps[] = {18000, 11700};

const int axisNormal[] = {1,-1};

//For Start of Non Grave Yard Square Bottom Left
const int squareStartBound[] = {2800,0};
const int squareEndBound[] = {14500,11700};
const int squareSeperation = 1462;
const int speedAxis = 12;

Servo myservo; 

int GLOBAL_POS[] = {UNKNOWN_POS, UNKNOWN_POS};

Stepper steppers [] = {Stepper(stepsPerRevolution, StepperPins[LONG][0], StepperPins[LONG][2], StepperPins[LONG][1], StepperPins[LONG][3]),
                       Stepper(stepsPerRevolution, StepperPins[SHORT][0], StepperPins[SHORT][2], StepperPins[SHORT][1], StepperPins[SHORT][3])};
void setup() {
  Serial.begin(9600);
  myservo.attach(endEffectPin);
  pinMode(limitSwitchPins[LONG], INPUT); 
  pinMode(limitSwitchPins[SHORT], INPUT); 
  steppers[SHORT].setSpeed(speedAxis);
  steppers[LONG].setSpeed(speedAxis);

  calibrate();

  goToSquare(2, 1);
  grabPiece(WHITE);
  goToSquare(2, 3);
  dropPiece(WHITE);
  goToSquare(3,6);
  grabPiece(BLACK);
  goToSquare(3,4);
  dropPiece(BLACK);
  goToSquare(3, 1);
  grabPiece(WHITE);
  goToSquare(3, 2);
  dropPiece(WHITE);
  goToSquare(2, 3);
  grabPiece(WHITE);
  goToSquare(-1, 0);
  dropPiece(WHITE);
  goToSquare(3,4);
  grabPiece(BLACK);
  goToSquare(2,3);
}

void homeAxis(int axis)
{
  //While Limit Switch is open
  while(digitalRead(limitSwitchPins[axis]) == HIGH)
  {
    steppers[axis].step(axisNormal[axis]);
    delay(1); 
  }

  GLOBAL_POS[axis] = 0;
}

void calibrate()
{
  dropPiece(WHITE);
  homeAxis(LONG);
  homeAxis(SHORT);
}

void goToXY(int longAxisPos, int shortAxisPos)
{
  steppers[LONG].step((longAxisPos - GLOBAL_POS[LONG])*(axisNormal[LONG]*-1));
  steppers[SHORT].step((shortAxisPos - GLOBAL_POS[SHORT])*(axisNormal[SHORT]*-1));
  
  GLOBAL_POS[LONG] = longAxisPos;
  GLOBAL_POS[SHORT] = shortAxisPos;
}

void grabPiece(int color)
{
  if(color == WHITE)
  {
    myservo.write(0);
  }
  else
  {
    myservo.write(180); 
  }
}

void dropPiece(int color)
{
  if(color == WHITE)
  {
    goToXY(GLOBAL_POS[LONG],GLOBAL_POS[SHORT] + (squareSeperation / 2));
  }
  else
  {
    goToXY(GLOBAL_POS[LONG],GLOBAL_POS[SHORT] - (squareSeperation / 2));
  }
  myservo.write(90);
}

void goToSquare(int pX, int pY)
{
  int x = squareStartBound[LONG] + (squareSeperation / 2) + (squareSeperation * pX);
  int y = squareStartBound[SHORT] + (squareSeperation / 2) + (squareSeperation * pY);
  goToXY(x,y);
}

void loop() {

}
