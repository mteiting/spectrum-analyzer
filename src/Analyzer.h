#pragma once
#include <Adafruit_NeoPixel.h>
#include <vector>
#include "band.h"

class IAnalyzer
{
public:
  IAnalyzer(){};
  virtual ~IAnalyzer(){};
  virtual std::vector<Band *> getBands() = 0;
  virtual void setBand(Band *newBand) = 0;
  virtual void loop() = 0;
};

class Analyzer : public IAnalyzer
{
private:
  Adafruit_NeoPixel *_ledControl;
  std::vector<Band *> _bands;

public:
  Analyzer(Adafruit_NeoPixel *ledControl);
  virtual ~Analyzer();
  std::vector<Band *> getBands() override;
  void setBand(Band *newBand) override;
  void loop() override;
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
  std::vector<Band *> getBands() override;
  void setBand(Band *newBand) override;
  void loop() override;
};