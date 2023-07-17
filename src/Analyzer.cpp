#include <Arduino.h>
#include "Analyzer.h"
#include "led.h"
#include "analyzerWiFi.h"
#include "tools.h"

constexpr uint8_t DEFAULT_BRIGHTNESS = 30; //[%]

void Analyzer::setup()
{
  StHtmlValues html{};
  html.u8Brightness = DEFAULT_BRIGHTNESS;
  setHtmlValues(html);

  for (const auto &band : _bands)
  {
    std::shared_ptr<Adafruit_NeoPixel> strip = band->getStrip();
    strip->begin();
    strip->show();
    strip->setBrightness(getHtmlValues().u8Brightness);
  }
}

std::vector<std::shared_ptr<Band>> Analyzer::getBands()
{
  return this->_bands;
}

void Analyzer::setBand(std::shared_ptr<Band> &newBand)
{
  if (newBand == nullptr)
    return;
  this->_bands.push_back(std::move(newBand));
}

void Analyzer::loop(std::vector<uint8_t> &newLevel)
{
  for (const auto &band : _bands)
  {
    std::shared_ptr<Adafruit_NeoPixel> strip = band->getStrip();
    strip->setBrightness(getHtmlValues().u8Brightness);
    band->setPeakLedDelay(getHtmlValues().u32PeakLedDelay);
    band->updateBandLevel(newLevel.at(band->getNumber()));

    uint16_t u16NumOfLed = band->getNumOfLEDs();
    for (uint16_t led = 0; led < u16NumOfLed; led++)
    {
      TstRGB rgb = band->getLedColor(led);
      strip->setPixelColor(band->getHardwareLedNumber(led),
                           strip->Color(rgb.red, rgb.green, rgb.blue));
    }
    strip->show();
  }
}