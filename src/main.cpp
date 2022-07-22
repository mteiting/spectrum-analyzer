#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "analyzerFFT.h"
#include "led.h"
#include "band.h"
#include "analyzerWiFi.h"

static Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
static Analyzer analyzer(&strip);

void setup()
{
  Serial.begin(115200);

  analyzerFFT_Setup();
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
  analyzerFFT_Task();
  analyzer.loop(getBandsFromFFT());
  WifiTask();
}