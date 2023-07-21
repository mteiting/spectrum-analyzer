#pragma once
#include <map>
#include <memory>
#include <Adafruit_NeoPixel.h>
#include "led.h"
#include "defaults.h"

enum class EnLedCountDir
{
  enLedCountDir_Top = 0,
  enLedCountDir_Down
};
class Band
{
private:
  std::shared_ptr<Adafruit_NeoPixel> _ledControl;

  std::map<uint16_t, TstRGB> _mLedColor;
  std::map<uint16_t, TstRGB> _mLevelColor;
  uint16_t _u16NumOfLEDs{0};
  uint16_t _u16LedOffset{0};
  uint16_t _u16PeakLED{0};
  uint8_t _u8Number{0};
  uint8_t _u8Level{0}; // in %
  double _dColorFader{0};
  double _dColorFaderResolution{DEFUALT_COLORFADER_RESOLUTION};
  EnLedCountDir _enCountDir{EnLedCountDir::enLedCountDir_Down};
  uint32_t _u32PeakLedDelay{0}; //[ms]
  uint32_t _timerPeakLedRefresh{0};

  void updatePeakLED(uint8_t u8CurrentLedLevel);

public:
  Band() = delete;
  Band(std::shared_ptr<Adafruit_NeoPixel> ledControl,
       uint8_t u8Number,
       uint16_t u16OffsetLED,
       uint16_t u16NumOfLEDs,
       EnLedCountDir enCountDir);
  Band(std::shared_ptr<Adafruit_NeoPixel> ledControl,
       uint8_t u8Number,
       uint16_t u16OffsetLED,
       uint16_t u16NumOfLEDs);
  virtual ~Band(){};

  inline std::shared_ptr<Adafruit_NeoPixel> &getStrip() { return _ledControl; }

  inline uint16_t getNumOfLEDs() { return _u16NumOfLEDs; }
  inline void setNumOfLEDs(uint16_t &newNumberOfLeds) { _u16NumOfLEDs = newNumberOfLeds; }

  inline void setColorFader(double dColorFader, double dColorFaderResolution)
  {
    _dColorFader = dColorFader;
    _dColorFaderResolution = dColorFaderResolution;
  }

  inline void getColorFader(double &dColorFader, double &dColorFaderResolution)
  {
    dColorFader = _dColorFader;
    dColorFaderResolution = _dColorFaderResolution;
  }

  uint16_t getLedOffset() { return _u16LedOffset; }
  void setLedOffset(uint16_t u16LedOffset) { _u16LedOffset = u16LedOffset; }

  uint8_t getNumber() { return _u8Number; }
  void setNumber(uint8_t newNumber) { _u8Number = newNumber; }

  uint32_t getPeakLedDelay() { return _u32PeakLedDelay; }
  void setPeakLedDelay(uint32_t u32NewDelay) { _u32PeakLedDelay = u32NewDelay; }

  TstRGB &getLedColor(uint16_t u16Number) { return _mLedColor[u16Number]; }
  uint8_t getLevel() { return _u8Level; }
  void setLedCountDir(EnLedCountDir enNewDir) { _enCountDir = enNewDir; }

  void updateBandLevel(uint8_t newLevel);
  uint16_t getHardwareLedNumber(uint16_t u16CurrentLed);
};