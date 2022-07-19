#include <Arduino.h>
#include "Analyzer.h"
#include "led.h"

constexpr uint8_t DEFAULT_BRIGHTNESS = 10;

Analyzer::Analyzer(Adafruit_NeoPixel *ledControl) : _ledControl(ledControl)
{
  if (_ledControl == nullptr)
    Serial.println("ledControl could was not created");
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
  _ledControl->begin();
  _ledControl->show();
  _ledControl->setBrightness(DEFAULT_BRIGHTNESS);
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

void Analyzer::loop()
{
  if (_ledControl == nullptr)
    return;

  for (const auto &strip : _bands)
  {
    strip->updateBandLevel((rand() % 100));

    uint16_t u16NumOfLed = strip->getNumOfLEDs();
    for (uint16_t led = 0; led < u16NumOfLed; led++)
    {
      TstRGB rgb = strip->getLedColor(led);
      uint16_t u16CurrentLED = strip->getHardwareLedNumber(led) + strip->getLedOffset();
      _ledControl->setPixelColor(u16CurrentLED, _ledControl->Color(rgb.red, rgb.green, rgb.blue));
    }
    _ledControl->show();
  }
}