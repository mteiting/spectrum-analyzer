#include <Arduino.h>
#include "Analyzer.h"
#include "led.h"

Analyzer::Analyzer(Adafruit_NeoPixel *ledControl) : _ledControl(ledControl)
{
  constexpr uint8_t DEFAULT_BRIGHTNESS = 10;
  _ledControl->begin();
  _ledControl->show();
  _ledControl->setBrightness(DEFAULT_BRIGHTNESS);
}

Analyzer::~Analyzer()
{
  if (_ledControl)
    delete (_ledControl);
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
  static auto offset = millis();
  auto now = millis();

  if ((now - offset) < 1000)
    return;

  offset = millis();
  uint8_t red, green, blue = 0;
  getRGBColor(red, green, blue);
  uint32_t color = _ledControl->Color(red, green, blue);

  for(const auto &strip : _bands)
  {
    _ledControl->fill(color, strip->getNumber(), strip->getNumOfLEDs());
    _ledControl->show();
    Serial.printf("set %u bis %u", strip->getNumber(), strip->getNumOfLEDs());
  }  
}