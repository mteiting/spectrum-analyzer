#include "led.h"
#include "tools.h"

static void rgbFading(TstRGB &color, double ratio)
{
  // we want to normalize ratio so that it fits in to 6 regions
  // where each region is 256 units long
  int normalized = int(ratio * 256 * 6);
  // find the region for this position
  int region = normalized / 256;
  // find the distance to the start of the closest region
  int x = normalized % 256;

  switch (region)
  {
  case 0:
    color.red = 255;
    color.green = 0;
    color.blue = 0;
    color.green += x;
    break;
  case 1:
    color.red = 255;
    color.green = 255;
    color.blue = 0;
    color.red -= x;
    break;
  case 2:
    color.red = 0;
    color.green = 255;
    color.blue = 0;
    color.blue += x;
    break;
  case 3:
    color.red = 0;
    color.green = 255;
    color.blue = 255;
    color.green -= x;
    break;
  case 4:
    color.red = 0;
    color.green = 0;
    color.blue = 255;
    color.red += x;
    break;
  case 5:
    color.red = 255;
    color.green = 0;
    color.blue = 255;
    color.blue -= x;
    break;
  }
}

void getRandomRGBColor(uint8_t &red, uint8_t &green, uint8_t &blue)
{
  red = rand() % 256;
  green = rand() % 256;
  blue = rand() % 256;
}

void getPeakLedColor(TstRGB &rgb)
{
  rgb.red = 255;
  rgb.green = 255;
  rgb.blue = 255;
}

void resetLedColor(TstRGB &rgb)
{
  const TstRGB RGB_AUS = {0, 0, 0};
  rgb = RGB_AUS;
}

/**
 * @brief color set with fading
 *
 * @param rgb
 * @param Level
 * @param dFader
 * @param dResolution
 */
void getRGB4Level(TstRGB &rgb,
                  uint16_t Level,
                  uint16_t maxLevel,
                  double dFader,
                  double dResolution)
{
  double value = dFader;
  double levelShifter = Level * dResolution / maxLevel;
  if ((levelShifter + value) > dResolution)
    value = dResolution - levelShifter;
  else
    value = levelShifter + value;

  rgbFading(rgb, value / dResolution);
}

/**
 * @brief standard color set
 *
 * @param rgb
 * @param Level
 */
void getRGB4Level(TstRGB &rgb, uint16_t Level)
{
  switch (Level)
  {
  case 0:
  case 1:
  case 2:
    rgb.blue = 0;
    rgb.green = 0;
    rgb.red = 255;
    break;
  case 3:
  case 4:
    rgb.blue = 0;
    rgb.green = 60;
    rgb.red = 255;
    break;
  case 5:
  case 6:
    rgb.blue = 0;
    rgb.green = 140;
    rgb.red = 255;
    break;
  case 7:
  case 8:
    rgb.blue = 0;
    rgb.green = 180;
    rgb.red = 255;
    break;
  case 9:
  case 10:
    rgb.blue = 0;
    rgb.green = 215;
    rgb.red = 255;
    break;
  case 11:
  case 12:
    rgb.blue = 0;
    rgb.green = 255;
    rgb.red = 200;
    break;
  case 13:
  case 14:
    rgb.blue = 0;
    rgb.green = 255;
    rgb.red = 100;
    break;
  case 15:
  case 16:
    rgb.blue = 0;
    rgb.green = 255;
    rgb.red = 0;
    break;
  case 17:
  case 18:
    rgb.blue = 255;
    rgb.green = 191;
    rgb.red = 0;
    break;
  case 19:
  case 20:
  default:
    rgb.blue = 255;
    rgb.green = 0;
    rgb.red = 0;
    break;
  }
}