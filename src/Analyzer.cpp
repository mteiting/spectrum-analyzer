#include <Arduino.h>
#include "Analyzer.h"
#include "led.h"

constexpr uint8_t DEFAULT_BRIGHTNESS = 10;

Analyzer::Analyzer(Adafruit_NeoPixel *ledControl) : _ledControl(ledControl)
{
  if(_ledControl == nullptr)
    Serial.println("ledControl could was not created");
}

Analyzer::~Analyzer()
{
  if (_ledControl)
    delete (_ledControl);
}

void Analyzer::setup()
{
  if(_ledControl == nullptr)
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
  if(newBand == nullptr)
    return;
  this->_bands.push_back(newBand);
}

void Analyzer::loop()
{
  if(_ledControl == nullptr)
    return;

  static auto offset = millis();
  auto now = millis();
  if ((now - offset) < 1000)
    return;

  offset = millis();
  for(const auto &strip : _bands)
  {
    uint8_t red, green, blue = 0;
    getRGBColor(red, green, blue);
    uint32_t color = _ledControl->Color(red, green, blue);

    _ledControl->fill(color, strip->getLedOffset(), strip->getNumOfLEDs());
    _ledControl->show();
  }  
}