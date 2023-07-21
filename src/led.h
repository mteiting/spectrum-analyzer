#pragma once

#include <Arduino.h>
#include <vector>

#define BAND_1_PIN 15
#define BAND_2_PIN 16
#define BAND_3_PIN 17
#define BAND_4_PIN 18
#define BAND_5_PIN 19
#define BAND_6_PIN 21
#define BAND_7_PIN 23

struct TstRGB
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

void getRandomRGBColor(uint8_t &red, uint8_t &green, uint8_t &blue);
void getRGB4Level(TstRGB &rgb, uint16_t Level);
void getRGB4Level(TstRGB &rgb, uint16_t Level, uint16_t maxLevel, double dFader, double dResolution);
void getPeakLedColor(TstRGB &rgb);
void resetLedColor(TstRGB &rgb);
