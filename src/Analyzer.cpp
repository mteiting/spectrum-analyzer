#include <Arduino.h>
#include "Analyzer.h"
#include "led.h"
#include "tools.h"

constexpr uint8_t DEFAULT_BRIGHTNESS = 30;
constexpr uint32_t DEFAULT_REFRESH_TIME_PEAK_LED = 20; // in ms

Analyzer::Analyzer(Adafruit_NeoPixel *ledControl) : _ledControl(ledControl),
                                                    _u32PeakLedDelay(DEFAULT_REFRESH_TIME_PEAK_LED),
                                                    _timerPeakLedRefresh(millis())
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

void Analyzer::loop(std::vector<uint8_t> &newLevel)
{
  if (_ledControl == nullptr)
    return;

  for (const auto &strip : _bands)
  {
    strip->updateBandLevel(newLevel.at(strip->getNumber()),
                           isTimeExpired(_timerPeakLedRefresh, _u32PeakLedDelay));

    uint16_t u16NumOfLed = strip->getNumOfLEDs();
    for (uint16_t led = 0; led < u16NumOfLed; led++)
    {
      TstRGB rgb = strip->getLedColor(led);
      _ledControl->setPixelColor(strip->getHardwareLedNumber(led),
                                 _ledControl->Color(rgb.red, rgb.green, rgb.blue));
    }
    _ledControl->show();
  }
}