#include <Arduino.h>

#define BTN_SPEED 35
#define BTN_ANGLE 32
#define BTN_AUTO_SEARCH     33
#define BTN_KEEP_DIRECTION     14
#define BTN_LEFT     12
#define BTN_RIGHT     13

#define PEDAL_LEFT 2 
#define PEDAL_RIGHT 4 

void setupButtons()
{
    pinMode(BTN_AUTO_SEARCH,INPUT_PULLDOWN);
    pinMode(BTN_KEEP_DIRECTION,INPUT_PULLDOWN);
    pinMode(BTN_LEFT,INPUT_PULLDOWN);
    pinMode(BTN_RIGHT,INPUT_PULLDOWN);

    pinMode(PEDAL_LEFT,INPUT_PULLDOWN);
    pinMode(PEDAL_RIGHT,INPUT_PULLDOWN);

}

void readButtons()
{
    auto speed = analogRead(BTN_SPEED);
    auto angle = analogRead(BTN_ANGLE);
    //auto btn1 = digitalRead(BTN_1);
    //auto btn2 = digitalRead(BTN_2);
    auto btn3 = digitalRead(BTN_LEFT);
    auto btn4 = digitalRead(BTN_RIGHT);
    auto pedalLeft = digitalRead(PEDAL_LEFT);
    auto pedalRight = digitalRead(PEDAL_RIGHT);
    //Serial.printf("Speed:%d angle:%d btns:%d %d %d %d %d %d\n",speed,angle,btn_1_state,btn2,btn3,btn4,pedalLeft,pedalRight);

}
bool IsAutoSearch()
{
     return digitalRead(BTN_AUTO_SEARCH) == 1;
}
bool IsKeepDirection()
{
     return digitalRead(BTN_KEEP_DIRECTION) == 1;
}
bool IsLeftPedalOn()
{
    return digitalRead(PEDAL_LEFT) == 1 || digitalRead(BTN_LEFT) == 1;
}
bool IsRightPedalOn()
{
    return digitalRead(PEDAL_RIGHT) == 1 || digitalRead(BTN_RIGHT) == 1;
}
float GetSpeed()
{
    auto speed = ((analogRead(BTN_SPEED) - 4095) * -1)/4095.0;
    //Serial.printf("Speed:%f\n",speed);
    return speed;
}
float GetAngle()
{
    auto speed = ((analogRead(BTN_ANGLE) - 4095) * -1)/4095.0;
    //Serial.printf("Speed:%f\n",speed);
    return speed;
}
bool IsPedals()
{
  return IsLeftPedalOn() || IsRightPedalOn();
}
bool IsBothPedals()
{
  return IsLeftPedalOn() && IsRightPedalOn();
}