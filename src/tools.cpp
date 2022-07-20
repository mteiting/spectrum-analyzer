#include "tools.h"


bool isTimeExpired(uint32_t &u32StartTime, uint32_t &u32TimeDuration)
{
  bool bReducePeakLed = false;
  if ((millis() - u32StartTime) > u32TimeDuration)
  {
    bReducePeakLed = true;
    u32StartTime = millis();
  }
  return bReducePeakLed;
}