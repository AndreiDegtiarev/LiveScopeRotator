#include <Arduino.h>

int PUL = 25; //define Pulse pin
int DIR = 26; //define Direction pin
int ENA = 27; //define Enable Pin

int cutSteps = 0;

void setupStepper()
{
    pinMode (PUL, OUTPUT);
    pinMode (DIR, OUTPUT);
    pinMode (ENA, OUTPUT);
}

float GetCurrentAngle()
{
      return cutSteps / (3.71 * 16) * 1.8;
}

void rotateStepperWhile(bool (*isKeepRotating)(),int (*getSpeed)(),bool (*isPositiveDirection)()) 
{
    while(isKeepRotating()) {
        auto speed = getSpeed();
        if(speed != 0)
        {
          auto isPositiveDir = isPositiveDirection();
          digitalWrite(DIR, isPositiveDir);
          digitalWrite(PUL, HIGH);
          delayMicroseconds(speed); // Adjust speed as necessary
          digitalWrite(PUL, LOW);
          delayMicroseconds(speed);
          cutSteps= cutSteps + 1 * (isPositiveDir?1:-1);
        }
      }
}
void rotateStepperDiff(float diffAngle,int speed) 
{

  long stepDifference = diffAngle / 1.8 * 3.71 * 16;
  //long stepDifference = steps - lastStep;
  int direction = stepDifference > 0 ? HIGH : LOW;
  /*Serial.print("\t");
  Serial.print(DIR);
  Serial.print("\t");
  Serial.print(stepDifference);*/
  //digitalWrite(DIR, direction);
  for (long i = 0; i < abs(stepDifference); i++) {
    digitalWrite(DIR, direction);
    digitalWrite(PUL, HIGH);
    delayMicroseconds(speed); // Adjust speed as necessary
    digitalWrite(PUL, LOW);
    delayMicroseconds(speed);
  }
}


bool rotateStepper(float angle) {
  static float lastAngle = 0;
  if(fabs(angle - lastAngle) < 1 )
    return false; 
  float diffAngle = angle - lastAngle; 
  
  
  Serial.print(lastAngle);Serial.print("\t");
  Serial.print(diffAngle);Serial.print("\t");

  diffAngle = min(abs(diffAngle),abs(diffAngle -360));
  Serial.print(diffAngle);Serial.print("\t");

  rotateStepperDiff(diffAngle,150);

  lastAngle = angle;
  return true;
}
