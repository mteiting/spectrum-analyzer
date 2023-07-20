#include <Arduino.h>
#include <memory>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "analyzerFFT.h"
#include "led.h"
#include "band.h"
#include "analyzerWiFi.h"
#include "tools.h"

std::shared_ptr<Analyzer> _analyzer = nullptr;

void setup()
{
  Serial.begin(115200);

  analyzerFFT_Setup();
  setupWifi();

  std::shared_ptr<Adafruit_NeoPixel> _strip1 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_1_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip2 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_2_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip3 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_3_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip4 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_4_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip5 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_5_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip6 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_6_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip7 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_7_PIN, NEO_GRB + NEO_KHZ800);

  std::shared_ptr<Band> band1 = std::make_shared<Band>(_strip1, 0, 0, 20);
  std::shared_ptr<Band> band2 = std::make_shared<Band>(_strip2, 1, 0, 20);
  std::shared_ptr<Band> band3 = std::make_shared<Band>(_strip3, 2, 0, 20);
  std::shared_ptr<Band> band4 = std::make_shared<Band>(_strip4, 3, 0, 20);
  std::shared_ptr<Band> band5 = std::make_shared<Band>(_strip5, 4, 0, 20);
  std::shared_ptr<Band> band6 = std::make_shared<Band>(_strip6, 5, 0, 20);
  std::shared_ptr<Band> band7 = std::make_shared<Band>(_strip7, 6, 0, 20);

  _analyzer = std::make_shared<Analyzer>();
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