#include <Arduino.h>
#include <iterator>
#include "band.h"
#include "analyzerWiFi.h"
#include "tools.h"
#include "defaults.h"

// #####################################################################
//                   public
// #####################################################################

Band::Band(std::shared_ptr<Adafruit_NeoPixel> ledControl,
           uint8_t u8Number,
           uint16_t u16OffsetLED,
           uint16_t u16NumOfLEDs,
           EnLedCountDir enCountDir)
    : _ledControl(ledControl),
      _u8Number(u8Number),
      _u16NumOfLEDs(u16NumOfLEDs),
      _u16LedOffset(u16OffsetLED),
      _enCountDir(enCountDir),
      _timerPeakLedRefresh(millis())
{
  for (uint16_t u16CurrentLed = 0; u16CurrentLed < _u16NumOfLEDs; u16CurrentLed++)
    this->_mLedColor.insert(std::pair<uint16_t, TstRGB>(u16CurrentLed, TstRGB()));
}

Band::Band(std::shared_ptr<Adafruit_NeoPixel> ledControl,
           uint8_t u8Number,
           uint16_t u16OffsetLED,
           uint16_t u16NumOfLEDs)
    : _ledControl(ledControl),
      _u8Number(u8Number),
      _u16NumOfLEDs(u16NumOfLEDs),
      _u16LedOffset(u16OffsetLED),
      _timerPeakLedRefresh(millis())
{
  for (uint16_t u16CurrentLed = 0; u16CurrentLed < _u16NumOfLEDs; u16CurrentLed++)
    this->_mLedColor.insert(std::pair<uint16_t, TstRGB>(u16CurrentLed, TstRGB()));
}

void Band::updateBandLevel(uint8_t newLevel)
{
  constexpr uint8_t PROZENT_MAX = 100; //[%]
  // wenn ueber 100% dann auf 100 begrenzen
  if (newLevel > PROZENT_MAX)
    newLevel = PROZENT_MAX;
  // level in %
  _u8Level = newLevel;
  // welche leds muessen eingeschaltet werden
  uint8_t currentLedLevel = this->_u16NumOfLEDs * newLevel / PROZENT_MAX;

  std::map<uint16_t, TstRGB>::iterator it;
  for (it = _mLedColor.begin(); it != _mLedColor.end(); it++)
  {
    if (it->first < currentLedLevel)
      getRGB4Level(it->second, it->first);
    else
      resetLedColor(it->second);
  }
  this->updatePeakLED(currentLedLevel);
}

uint16_t Band::getHardwareLedNumber(uint16_t u16CurrentLed)
{
  if (u16CurrentLed > this->_u16NumOfLEDs)
    return 0;

  uint16_t u16HwLedNum = 0;
  switch (_enCountDir)
  {
  case EnLedCountDir::enLedCountDir_Down:
    u16HwLedNum = u16CurrentLed;
    break;
  case EnLedCountDir::enLedCountDir_Top:
    u16HwLedNum = this->_u16NumOfLEDs - (u16CurrentLed + 1);
    break;
  }
  return u16HwLedNum + _u16LedOffset;
}

// #####################################################################
//                   private
// #####################################################################

/**
 * @brief berechnet die peak led
 *
 * @param u8CurrentLedLevel letzte noch eingeschaltete led des bandes
 */
void Band::updatePeakLED(uint8_t u8CurrentLedLevel)
{
  bool bUpdatePeakLED = isTimeExpired(_timerPeakLedRefresh, _u32PeakLedDelay);

  if (_u16PeakLED <= u8CurrentLedLevel)
  {
    _u16PeakLED = u8CurrentLedLevel;
    _timerPeakLedRefresh = millis();
  }
  else if (bUpdatePeakLED)
  { // peak led hoeher als aktuelles level, also langsam runter gehen, bUpdatePeakLED = true wenn delay abgelaufen
    resetLedColor(_mLedColor[_u16PeakLED]);
    _timerPeakLedRefresh = millis();
    if (_u16PeakLED)
      _u16PeakLED -= 1;
  }
  else
  {
  }
  getPeakLedColor(_mLedColor[_u16PeakLED]);
}