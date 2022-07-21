#pragma once
#include <map>
#include "led.h"

enum class EnLedCountDir
{
  enLedCountDir_Top = 0,
  enLedCountDir_Down
};

class Band
{
private:
  std::map<uint16_t, TstRGB> _mLedColor;
  uint16_t _u16NumOfLEDs;
  uint16_t _u16LedOffset;
  uint16_t _u16PeakLED;
  uint8_t _u8Number;
  uint8_t _u8Level; // in %
  EnLedCountDir _enCountDir;

  void updatePeakLED(uint8_t u8CurrentLedLevel, bool bUpdatePeakLED);

public:
  Band(){};
  Band(uint8_t u8Number, uint16_t u16OffsetLED, uint16_t u16NumOfLEDs, EnLedCountDir enCountDir);
  virtual ~Band(){};

  uint16_t getNumOfLEDs();
  void setNumOfLEDs(uint16_t &);

  uint16_t getLedOffset();
  void setLedOffset(uint16_t u16LedOffset);

  uint8_t getNumber();
  void setNumber(uint8_t newNumber);

  uint8_t getLevel();
  void updateBandLevel(uint8_t newLevel, bool bUpdatePeakLED);

  TstRGB &getLedColor(uint16_t u16Led);
  void setLedCountDir(EnLedCountDir enNewDir);
  uint16_t getHardwareLedNumber(uint16_t u16CurrentLed);
};