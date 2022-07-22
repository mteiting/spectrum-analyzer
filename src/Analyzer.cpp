#include <Arduino.h>
#include "Analyzer.h"
#include "led.h"
#include "analyzerWiFi.h"
#include "tools.h"

constexpr uint8_t DEFAULT_BRIGHTNESS = 30; //[%]

Analyzer::Analyzer(Adafruit_NeoPixel *ledControl) : _ledControl(ledControl)
{
  if (_ledControl == nullptr)
    Serial.println("was not able to create ledControl");
}

Analyzer::~Analyzer()
{
  if (_ledControl)
    delete (_ledControl);
}

void Analyzer::setup()
{
  if (_ledControl == nullptr)
    return;
  StHtmlValues html{};
  html.u8Brightness = DEFAULT_BRIGHTNESS;
  setHtmlValues(html);

  _ledControl->begin();
  _ledControl->show();
  _ledControl->setBrightness(getHtmlValues().u8Brightness);
}

std::vector<Band *> Analyzer::getBands()
{
  return this->_bands;
}

void Analyzer::setBand(Band *newBand)
{
  if (newBand == nullptr)
    return;
  this->_bands.push_back(newBand);
}

void Analyzer::loop(std::vector<uint8_t> &newLevel)
{
  if (_ledControl == nullptr)
    return;

  _ledControl->setBrightness(getHtmlValues().u8Brightness);
  for (const auto &strip : _bands)
  {
    strip->setPeakLedDelay(getHtmlValues().u32PeakLedDelay);
    strip->updateBandLevel(newLevel.at(strip->getNumber()));

    uint16_t u16NumOfLed = strip->getNumOfLEDs();
    for (uint16_t led = 0; led < u16NumOfLed; led++)
    {
      TstRGB rgb = strip->getLedColor(led);
      _ledControl->setPixelColor(strip->getHardwareLedNumber(led),
                                 _ledControl->Color(rgb.red, rgb.green, rgb.blue));
    }
  }
  _ledControl->show();
}