#include <Arduino.h>
#include <memory>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "analyzerFFT.h"
#include "led.h"
#include "band.h"
#include "analyzerWiFi.h"
#include "tools.h"

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

void simulationTask()
{
  static std::vector<uint8_t> vSimValues(BANDS, 0);
  static uint32_t u32TimerSimulation = millis();

  if (false == isTimeExpired(u32TimerSimulation, 200))
    return;
  for (auto &band : vSimValues)
  {
    band = (uint8_t)(1 + (rand() % 100));
  }
  _analyzer->loop(vSimValues);
}

void ledTest()
{
  constexpr uint8_t STEPS = 5;
  static std::vector<uint8_t> vSimValues(BANDS, 0);
  static uint32_t u32TimerSimulation = millis();
  static uint8_t u8LastBand = 0;
  static uint8_t u8CurrentBand = 0;

  if (vSimValues[u8LastBand] && u8CurrentBand != u8LastBand)
    vSimValues[u8LastBand] -= STEPS;

  vSimValues[u8CurrentBand] += STEPS;
  if (vSimValues[u8CurrentBand] >= 100)
  {
    u8LastBand = u8CurrentBand;
    u8CurrentBand++;
    if (u8CurrentBand >= BANDS)
      u8CurrentBand = 0;
  }
  _analyzer->loop(vSimValues);
}

void loop()
{
  WifiTask();

  if (getHtmlValues().bSimulationStart)
  {
    simulationTask();
  }
  else if (getHtmlValues().bLedTestStart)
  {
    ledTest();
  }
  else
  {
    analyzerFFT_Task();
    _analyzer->loop(getBandsFromFFT());
  }
}