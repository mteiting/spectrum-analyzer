#pragma once

class Band
{
private:
  uint16_t _u16MinFreq;
  uint16_t _u16MaxFreq;
  uint16_t _u16NumOfLEDs;
  uint8_t _u8Number;

public:
  Band(){};
  Band(uint8_t u8Number, uint16_t u16NumOfLEDs);
  Band(uint8_t u8Number, uint16_t u16MinFreq, uint16_t u16MaxFreq, uint16_t u16NumOfLEDs);
  virtual ~Band(){};
  uint16_t getNumOfLEDs();
  void setNumOfLEDs(uint16_t &);
  void getFreqBand(uint16_t &minFreq, uint16_t &maxFreq);
  void setFreqBand(uint16_t &minFreq, uint16_t &maxFreq);
  uint8_t getNumber();
  void setNumber(uint8_t newNumber);
};