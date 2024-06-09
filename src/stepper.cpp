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
      return cutSteps / (3.71 * 16 * 2) * 1.8;
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
