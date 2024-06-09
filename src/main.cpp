// Basic demo for accelerometer readings from Adafruit ICM20948

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "declarations.h"
#include "KalmanFilter.h"

enum CurMode
{
   ManualMode,
   AutoSearchMode,
   KeepDirectionMode
};

int maxSpeed = 1500;
CurMode curMode = ManualMode;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  setup_ICM20948();
  Serial.println();

  setupStepper();
  setupButtons();
}

int getSpeed()
{
    return (1-GetSpeed()) * maxSpeed;
}
bool isKeepRotationPedal()
{
    auto isLeftPedalOn = IsLeftPedalOn();
    auto isRightPedalOn = IsRightPedalOn();
    return (isLeftPedalOn && !isRightPedalOn) || (!isLeftPedalOn && isRightPedalOn);
}
bool isPositiveDirectionPedal()
{
  return IsLeftPedalOn();
}
float curAutoSearchCentralAngle = 0;
float curTargetYawAngle = 0;
float curStepperYawDelta = 0;
float curTargetStepperAngle = 0;


bool isKeepRotationAutoSearch() 
{
    return (!IsPedals() && !IsKeepDirection());
} 
bool isPositiveDirectionAutoSearch()
{
  auto angle = GetCurrentAngle();
  static bool isPositiveDir = true;
  auto maxAngle = GetAngle() * 270/2;
  if(angle < -maxAngle + curAutoSearchCentralAngle)
    isPositiveDir = true;
  if(angle > maxAngle +curAutoSearchCentralAngle)
    isPositiveDir = false;
  return isPositiveDir;
}
KalmanFilter kalmanFilter(1.e-2,1505,0);
float curCorrectionAngle = 0;
float calcCorrectionAngle()
{
  auto angle = GetCurrentAngle();
  auto yaw = getYaw();
  auto aupdatedYaw = kalmanFilter.update(yaw);  
  auto corYaw =  aupdatedYaw - curTargetYawAngle;
  curCorrectionAngle = (curTargetStepperAngle - corYaw) - angle;
  //Serial.printf("curTargetYawAngle:%f yaw:%f angle:%f curCorrectionAngle:%f ",curTargetYawAngle,yaw,angle,curCorrectionAngle);
  if(curCorrectionAngle > 180)
      curCorrectionAngle -= 360;
  else if(curCorrectionAngle < -180)
    curCorrectionAngle += 360;
  //Serial.printf("%f %f %f\n",yaw,aupdatedYaw,curCorrectionAngle);

  //Serial.printf("%f \n",curCorrectionAngle);
  return curCorrectionAngle;
}
bool isKeepRotationKeepDirection()
{
  return !IsPedals() && !IsAutoSearch();     
}
int getSpeedKeepDirection()
{
  if(fabs(calcCorrectionAngle())<1)
    return 0;
  //Serial.printf("%f \n",curCorrectionAngle);
  return 100;
}
bool isPositiveDirectionKeepDirection()
{

  auto isPositive = curCorrectionAngle > 0;
  //Serial.printf("Target yaw angle:%f delat %f Cur stepper angle:%f yaw:%f isPositive:%d\n",curTargetYawAngle,curStepperYawDelta,angle,yaw,isPositive);
  return isPositive;
}
int64_t timeLastModeChange = 0;
void loop() {

  if(IsAutoSearch() || curMode == AutoSearchMode)
  {
    Serial.println("AutoSearch");
    curMode = AutoSearchMode;
    curAutoSearchCentralAngle = GetCurrentAngle();
    rotateStepperWhile(isKeepRotationAutoSearch,getSpeed,isPositiveDirectionAutoSearch);
  }
  else if (IsKeepDirection() || curMode == KeepDirectionMode)
  {
    Serial.println("KeepDirection");
    curMode = KeepDirectionMode;
    curTargetYawAngle = getYaw();
    curStepperYawDelta = curTargetYawAngle - GetCurrentAngle();
    curTargetStepperAngle = GetCurrentAngle();
    kalmanFilter.reset(curTargetYawAngle);
    rotateStepperWhile(isKeepRotationKeepDirection,getSpeedKeepDirection,isPositiveDirectionKeepDirection);
  }
  auto crTime = esp_timer_get_time();
  if(IsBothPedals() && (crTime - timeLastModeChange)> 1e6 )
  {
    if(curMode == ManualMode)
    {
      curMode = KeepDirectionMode;
      timeLastModeChange = crTime;
    }
    else
    {
      Serial.println("ManualMode");
      curMode = ManualMode;
      timeLastModeChange = crTime;
    }
  }
  rotateStepperWhile(isKeepRotationPedal,getSpeed,isPositiveDirectionPedal);
}

