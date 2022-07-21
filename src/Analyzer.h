#pragma once
#include <Adafruit_NeoPixel.h>
#include <vector>
#include "band.h"

class IAnalyzer
{
public:
  IAnalyzer(){};
  virtual ~IAnalyzer(){};
  virtual void setup() = 0;
  virtual std::vector<Band *> getBands() = 0;
  virtual void setBand(Band *newBand) = 0;
  virtual void loop(std::vector<uint8_t> &newLevel) = 0;
};

class Analyzer : public IAnalyzer
{
private:
  Adafruit_NeoPixel *_ledControl;
  std::vector<Band *> _bands;
  uint32_t _u32PeakLedDelay;
  uint32_t _timerPeakLedRefresh;

public:
  Analyzer(Adafruit_NeoPixel *ledControl);
  virtual ~Analyzer();
  void setup() override;
  std::vector<Band *> getBands() override;
  void setBand(Band *newBand) override;
  void loop(std::vector<uint8_t> &newLevel) override;
};

// For future stuff
class AnalyzerIP : public IAnalyzer
{
private:
  IPAddress _ipadress;
  std::vector<Band *> _bands;
  uint32_t _u32PeakLedDelay;

public:
  AnalyzerIP(IPAddress &ipadress);
  virtual ~AnalyzerIP(){};
  void setup() override;
  std::vector<Band *> getBands() override;
  void setBand(Band *newBand) override;
  void loop(std::vector<uint8_t> &newLevel) override;
};