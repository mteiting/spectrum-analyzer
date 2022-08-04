#pragma once
#include <Adafruit_NeoPixel.h>
#include <vector>
#include <memory>
#include "band.h"

class IAnalyzer
{
public:
  IAnalyzer(){};
  virtual ~IAnalyzer(){};
  virtual void setup() = 0;
  virtual std::vector<std::shared_ptr<Band>> getBands() = 0;
  virtual void setBand(std::shared_ptr<Band> &newBand) = 0;
  virtual void loop(std::vector<uint8_t> &newLevel) = 0;
};

class Analyzer : public IAnalyzer
{
private:
  std::shared_ptr<Adafruit_NeoPixel> _ledControl;
  std::vector<std::shared_ptr<Band>> _bands;

public:
  Analyzer(std::shared_ptr<Adafruit_NeoPixel> ledControl);
  virtual ~Analyzer();
  void setup() override;
  std::vector<std::shared_ptr<Band>> getBands() override;
  void setBand(std::shared_ptr<Band> &newBand) override;
  void loop(std::vector<uint8_t> &newLevel) override;
};

// For future stuff
class AnalyzerIP : public IAnalyzer
{
private:
  IPAddress _ipadress;
  std::vector<std::shared_ptr<Band>> _bands;

public:
  AnalyzerIP(IPAddress &ipadress);
  virtual ~AnalyzerIP(){};
  void setup() override;
  std::vector<std::shared_ptr<Band>> getBands() override;
  void setBand(std::shared_ptr<Band> &newBand) override;
  void loop(std::vector<uint8_t> &newLevel) override;
};