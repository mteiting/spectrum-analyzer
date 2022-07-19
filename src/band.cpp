#include <Arduino.h>
#include "band.h"

Band::Band(uint8_t u8Number, uint16_t u16MinFreq, uint16_t u16MaxFreq, uint16_t u16OffsetLED, uint16_t u16NumOfLEDs) : _u8Number(u8Number),
                                                                                                                       _u16NumOfLEDs(u16NumOfLEDs),
                                                                                                                       _u16LedOffset(u16OffsetLED),
                                                                                                                       _u16MinFreq(u16MinFreq),
                                                                                                                       _u16MaxFreq(u16MaxFreq)
{
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

void Band::getFreqBand(uint16_t &minFreq, uint16_t &maxFreq)
{
  minFreq = this->_u16MinFreq;
  maxFreq = this->_u16MaxFreq;
}

void Band::setFreqBand(uint16_t &minFreq, uint16_t &maxFreq)
{
  this->_u16MaxFreq = maxFreq;
  this->_u16MinFreq = minFreq;
}

uint8_t Band::getNumber()
{
  return this->_u8Number;
}

void Band::setNumber(uint8_t newNumber)
{
  this->_u8Number = newNumber;
}