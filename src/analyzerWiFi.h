#pragma once

struct StHtmlValues
{
  bool bScanForWifi = false;
  uint8_t u8Brightness = 0;
  uint8_t u8PeakLedDelay = 0;
};

String getSSID();
void setupWifi();
StHtmlValues &getHtmlValues();
void setHtmlValues(StHtmlValues& newHtmlValues);
void WifiTask();