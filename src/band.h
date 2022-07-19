#pragma once

class Band
{
private:
  std::map<uint16_t, TstRGB> _mLedColor;
  uint16_t _u16MinFreq;
  uint16_t _u16MaxFreq;
  uint16_t _u16NumOfLEDs;
  uint16_t _u16LedOffset;
  uint8_t _u8Number;

public:
  Band(){};
  Band(uint8_t u8Number, uint16_t u16MinFreq, uint16_t u16MaxFreq, uint16_t u16OffsetLED, uint16_t u16NumOfLEDs);
  virtual ~Band(){};

  uint16_t getNumOfLEDs();
  void setNumOfLEDs(uint16_t &);

  void getFreqBand(uint16_t &minFreq, uint16_t &maxFreq);
  void setFreqBand(uint16_t &minFreq, uint16_t &maxFreq);

  uint16_t getLedOffset();
  void setLedOffset(uint16_t u16LedOffset);

  uint8_t getNumber();
  void setNumber(uint8_t newNumber);

  void updateBandLevel(uint16_t newLevel);

  TstRGB& getLedColor(uint16_t u16Number);
};