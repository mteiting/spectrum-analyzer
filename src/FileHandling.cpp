#include <Preferences.h>
#include "analyzerWiFi.h"
#include "defaults.h"

static Preferences preferences;

void FileHandlingSetup()
{
  preferences.begin("analyzer", false);

  StHtmlValues stHtmlValues = getHtmlValues();
  stHtmlValues.fGain = preferences.getFloat("fGain", DEFAULT_GAIN);
  stHtmlValues.u32PeakLedDelay = preferences.getLong("peakLEDelay", DEFAULT_REFRESH_TIME_PEAK_LED);
  stHtmlValues.u8Brightness = preferences.getUChar("Brightness", DEFAULT_BRIGHTNESS);
  stHtmlValues.wifiSSID = preferences.getString("ssid", "");
  stHtmlValues.wifiPW = preferences.getString("password", "");
  stHtmlValues.bFading = preferences.getBool("bFading", false);
  stHtmlValues.dColorFadeOffset = preferences.getDouble("bColorOffset", false);
  preferences.end();
  setHtmlValues(stHtmlValues);
}

void FileHandlingSaveHtml(StHtmlValues &newValues)
{
  preferences.begin("analyzer", false);
  preferences.putFloat("fGain", newValues.fGain);
  preferences.putLong("peakLEDelay", newValues.u32PeakLedDelay);
  preferences.putUChar("Brightness", newValues.u8Brightness);
  preferences.putString("ssid", newValues.wifiSSID);
  preferences.putString("password", newValues.wifiPW);
  preferences.putBool("bFading", newValues.bFading);
  preferences.putDouble("bColorOffset", newValues.dColorFadeOffset);
  preferences.end();
}