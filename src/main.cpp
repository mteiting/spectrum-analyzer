#include <Arduino.h>
// #include <Wire.h>
#include <SparkFun_ADS1015_Arduino_Library.h>
#include <arduinoFFT.h>
#include <sstream>
#include <algorithm>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "led.h"
#include "band.h"
#include "analyzerWiFi.h"

// Declare our NeoPixel strip object:
static Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
static Analyzer analyzer(&strip);

void setup()
{
  Serial.begin(115200);

  setupWifi();

  analyzer.setup();
  Band *band = new Band(0, 0, 20, EnLedCountDir::enLedCountDir_Top);
  Band *band2 = new Band(1, 20, 20, EnLedCountDir::enLedCountDir_Down);
  Band *band3 = new Band(2, 40, 20, EnLedCountDir::enLedCountDir_Top);
  Band *band4 = new Band(3, 60, 20, EnLedCountDir::enLedCountDir_Down);
  Band *band5 = new Band(4, 80, 20, EnLedCountDir::enLedCountDir_Top);
  Band *band6 = new Band(5, 100, 20, EnLedCountDir::enLedCountDir_Down);
  Band *band7 = new Band(6, 120, 20, EnLedCountDir::enLedCountDir_Top);
  analyzer.setBand(band);
  analyzer.setBand(band2);
  analyzer.setBand(band3);
  analyzer.setBand(band4);
  analyzer.setBand(band5);
  analyzer.setBand(band6);
  analyzer.setBand(band7);
}

void loop()
{
  constexpr uint16_t REFRESH_RATE_MS = 1;

  static auto offset = millis();
  if ((millis() - offset) > REFRESH_RATE_MS)
  {
    analyzer.loop(bands_normalized);
    offset = millis();
  }

  WifiTask();
}