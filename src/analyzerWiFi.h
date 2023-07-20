#pragma once

#include "defaults.h"

struct StHtmlValues
{
  bool bScanForWifi{false};
  bool bSimulationStart{false};
  bool bLedTestStart{false};
  uint8_t u8Brightness{DEFAULT_BRIGHTNESS};
  float fGain{DEFAULT_GAIN};
  uint32_t u32PeakLedDelay{DEFAULT_REFRESH_TIME_PEAK_LED};
  String wifiSSID{""};
  String wifiPW{""};
};

String getSSID();
void setupWifi();
StHtmlValues &getHtmlValues();
void setHtmlValues(StHtmlValues &newHtmlValues);
void WifiTask();