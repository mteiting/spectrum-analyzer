#include "led.h"

void getRandomRGBColor(uint8_t &red, uint8_t &green, uint8_t &blue)
{
  red = rand() % 256;
  green = rand() % 256;
  blue = rand() % 256;
}

void getRGB4Level(TstRGB &rgb, uint16_t Level)
{
  switch (Level)
  {
  case 0: case 1: case 2: 
    rgb.blue = 0;
    rgb.green = 0;
    rgb.red = 255;
    break;
  case 3: case 4:
    rgb.blue = 48;
    rgb.green = 48;
    rgb.red = 255;
    break;
  case 5: case 6:
    rgb.blue = 0;
    rgb.green = 69;
    rgb.red = 255;
    break;
  case 7: case 8:
    rgb.blue = 0;
    rgb.green = 140;
    rgb.red = 255;
    break;
  case 9: case 10:
    rgb.blue = 0;
    rgb.green = 215;
    rgb.red = 255;
    break;
  case 11: case 12:
    rgb.blue = 0;
    rgb.green = 255;
    rgb.red = 255;
    break;
  case 13: case 14:
    rgb.blue = 0;
    rgb.green = 255;
    rgb.red = 0;
    break;
  case 15: case 16:
    rgb.blue = 0;
    rgb.green = 255;
    rgb.red = 100;
    break;
  case 17: case 18:
    rgb.blue = 255;
    rgb.green = 191;
    rgb.red = 0;
    break;
  case 19: case 20: default:
    rgb.blue = 255;
    rgb.green = 0;
    rgb.red = 0;
    break;
  }
}