#pragma once

#include <Arduino.h>

#define LED_PIN 15
#define LED_COUNT 100


struct TstRGB
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

void getRandomRGBColor(uint8_t &red, uint8_t &green, uint8_t &blue);
void getRGB4Level(TstRGB &rgb, uint16_t Level);