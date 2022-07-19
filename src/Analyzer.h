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
  virtual void loop(int* bands_normalized) = 0;
};

class Analyzer : public IAnalyzer
{
private:
  Adafruit_NeoPixel *_ledControl;
  std::vector<Band *> _bands;

public:
  Analyzer(Adafruit_NeoPixel *ledControl);
  virtual ~Analyzer();
  void setup() override;
  std::vector<Band *> getBands() override;
  void setBand(Band *newBand) override;
  void loop(int* bands_normalized) override;
};

//For future stuff
class AnalyzerIP : public IAnalyzer
{
private:
  IPAddress _ipadress;
  std::vector<Band *> _bands;

public:
  AnalyzerIP(IPAddress &ipadress);
  virtual ~AnalyzerIP(){};
  void setup() override;
  std::vector<Band *> getBands() override;
  void setBand(Band *newBand) override;
  void loop(int* bands_normalized) override;
};