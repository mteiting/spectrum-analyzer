#include <Arduino.h>
#include <iterator>

#include "band.h"

Band::Band(uint8_t u8Number, uint16_t u16OffsetLED, uint16_t u16NumOfLEDs, EnLedCountDir enCountDir) : _u8Number(u8Number),
                                                                                                       _u16NumOfLEDs(u16NumOfLEDs),
                                                                                                       _u16LedOffset(u16OffsetLED),
                                                                                                       _enCountDir(enCountDir)
{
  for (uint16_t u16CurrentLed; u16CurrentLed < _u16NumOfLEDs; u16CurrentLed++)
    this->_mLedColor.insert(std::pair<uint16_t, TstRGB>(u16CurrentLed, TstRGB()));
}

uint16_t Band::getNumOfLEDs()
{
  return this->_u16NumOfLEDs;
}

void Band::setLedOffset(uint16_t u16LedOffset)
{
  this->_u16LedOffset = u16LedOffset;
}

uint16_t Band::getLedOffset()
{
  return this->_u16LedOffset;
}

void Band::setNumOfLEDs(uint16_t &newNumberOfLeds)
{
  this->_u16NumOfLEDs = newNumberOfLeds;
}

uint8_t Band::getNumber()
{
  return this->_u8Number;
}

void Band::setNumber(uint8_t newNumber)
{
  this->_u8Number = newNumber;
}

uint8_t Band::getLevel()
{
  return this->_u8Level;
}

void Band::updateBandLevel(uint8_t newLevel)
{
  const TstRGB RGB_AUS = {0, 0, 0};
  // level in %
  _u8Level = newLevel;
  // welche leds muessen eingeschaltet werden
  uint8_t currentLedLevel = this->_u16NumOfLEDs * newLevel / 100;

  std::map<uint16_t, TstRGB>::iterator it;
  for (it = _mLedColor.begin(); it != _mLedColor.end(); it++)
  {
    if (it->first < currentLedLevel)
      getRGB4Level(it->second, it->first);
    else
      it->second = RGB_AUS;
  }
}

TstRGB &Band::getLedColor(uint16_t u16Number)
{
  return this->_mLedColor[u16Number];
}

void Band::setLedCountDir(EnLedCountDir enNewDir)
{
  this->_enCountDir = enNewDir;
}

uint16_t Band::getHardwareLedNumber(uint16_t u16CurrentLed)
{
  if(u16CurrentLed > this->_u16NumOfLEDs)
    return 0;

  uint16_t u16HwLedNum = 0;
  switch (_enCountDir)
  {
  case EnLedCountDir::enLedCountDir_Down:
    u16HwLedNum = u16CurrentLed;
    break;
  case EnLedCountDir::enLedCountDir_Top:
    u16HwLedNum = this->_u16NumOfLEDs - u16CurrentLed;
    break;
  }
  return u16HwLedNum;
}