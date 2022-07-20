#include "tools.h"


bool isTimeExpired(uint32_t &u32StartTime, uint32_t &u32TimeDuration)
{
  bool bExpired = false;
  if ((millis() - u32StartTime) > u32TimeDuration)
  {
    bExpired = true;
    u32StartTime = millis();
  }
  return bExpired;
}