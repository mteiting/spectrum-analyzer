#include "led.h"

void getRGBColor(uint8_t &red, uint8_t &green, uint8_t &blue)
{
  red = rand() % 256;
  green = rand() % 256;
  blue = rand() % 256;
}
