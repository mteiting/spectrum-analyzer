#include <Arduino.h>
#include <memory>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "analyzerFFT.h"
#include "led.h"
#include "band.h"
#include "analyzerWiFi.h"

std::shared_ptr<Adafruit_NeoPixel> _strip = nullptr;
std::shared_ptr<Analyzer> _analyzer = nullptr;

void setup()
{
  Serial.begin(115200);

  analyzerFFT_Setup();
  setupWifi();

  _strip = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
  _analyzer = std::make_shared<Analyzer>();

  std::shared_ptr<Band> band1 = std::make_shared<Band>(_strip, 0, 0, 20, EnLedCountDir::enLedCountDir_Top);
  std::shared_ptr<Band> band2 = std::make_shared<Band>(_strip, 1, 20, 20, EnLedCountDir::enLedCountDir_Down);
  std::shared_ptr<Band> band3 = std::make_shared<Band>(_strip, 2, 40, 20, EnLedCountDir::enLedCountDir_Top);
  std::shared_ptr<Band> band4 = std::make_shared<Band>(_strip, 3, 60, 20, EnLedCountDir::enLedCountDir_Down);
  std::shared_ptr<Band> band5 = std::make_shared<Band>(_strip, 4, 80, 20, EnLedCountDir::enLedCountDir_Top);
  std::shared_ptr<Band> band6 = std::make_shared<Band>(_strip, 5, 100, 20, EnLedCountDir::enLedCountDir_Down);
  std::shared_ptr<Band> band7 = std::make_shared<Band>(_strip, 6, 120, 20, EnLedCountDir::enLedCountDir_Top);
  _analyzer->setBand(band1);
  _analyzer->setBand(band2);
  _analyzer->setBand(band3);
  _analyzer->setBand(band4);
  _analyzer->setBand(band5);
  _analyzer->setBand(band6);
  _analyzer->setBand(band7);
  _analyzer->setup();
}

void loop()
{
  analyzerFFT_Task();
  _analyzer->loop(getBandsFromFFT());
  WifiTask();
}