#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "AsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include "analyzerWiFi.h"
#include "html.h"
#include "defaults.h"
#include "FileHandling.h"

const char *ssid = "SpectrumAnalyzer";
const char *password = "1234567890";

static AsyncWebServer server(80);
static StHtmlValues mglHtmlValues = {};
static std::string mglsWifiList = "";

static void scanForWifiNetworks()
{
  mglHtmlValues.bScanForWifi = false;

  uint16_t u16NumOfWifiNetworks = WiFi.scanNetworks();
  for (uint8_t u8CurrentWifi = 0; u8CurrentWifi < u16NumOfWifiNetworks; u8CurrentWifi++)
  {
    std::string newWifi = std::string(WiFi.SSID(u8CurrentWifi).c_str());
    mglsWifiList.append(newWifi);
    mglsWifiList += "##";
  }
}

static void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

static String sliderBrightnessValue(const String &var)
{
  if (var.equals(HtmlInput_Brightness))
    return String(mglHtmlValues.u8Brightness);
  return String();
}

static void setupServer()
{
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  server.on("/get_peakDelay", HTTP_GET, [](AsyncWebServerRequest *request)
            {
            if (request->hasParam(PARAM_INPUT)){
              mglHtmlValues.u32PeakLedDelay = request->getParam(PARAM_INPUT)->value().toInt();
              FileHandlingSaveHtml(mglHtmlValues);
            }
            request->send(200, "text/html", index_html); 
            request->redirect("/"); });

  server.on("/get_micGain", HTTP_GET, [](AsyncWebServerRequest *request)
            {
            if (request->hasParam(PARAM_INPUT)){
              mglHtmlValues.fGain = DEFAULT_GAIN + (request->getParam(PARAM_INPUT)->value().toFloat() / 100);
              FileHandlingSaveHtml(mglHtmlValues);
            }
            request->send(200, "text/html", index_html); 
            request->redirect("/"); });

  server.on("/test_start", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
            mglHtmlValues.bLedTestStart = !mglHtmlValues.bLedTestStart;
            mglHtmlValues.bSimulationStart = false;
            request->send(200, "text/html", index_html); 
            request->redirect("/"); });

  server.on("/sim_start", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
            mglHtmlValues.bSimulationStart = !mglHtmlValues.bSimulationStart;
            mglHtmlValues.bLedTestStart = false;
            request->send(200, "text/html", index_html); 
            request->redirect("/"); });

  server.on("/scan_wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
            mglHtmlValues.bScanForWifi = true;
            request->send(200, "text/html", index_html); 
            request->redirect("/"); });

  server.on("/get_wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
            if(mglsWifiList.empty())
              return;
            request->send(200, "text/plain", mglsWifiList.c_str()); 
            mglsWifiList.erase(); });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, sliderBrightnessValue); });

  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request)
            {
            if (request->hasParam(PARAM_INPUT)) {
              String sliderValue = request->getParam(PARAM_INPUT)->value();
              mglHtmlValues.u8Brightness = sliderValue.toInt();
              FileHandlingSaveHtml(mglHtmlValues);
            }
            request->send(200, "text/plain", "OK"); });

  server.onNotFound(notFound);
  server.begin();
}

static void setupAccessPoint()
{
  Serial.println("init AP ...");
  WiFi.mode(WIFI_AP_STA);
  if (true == WiFi.softAP(getSSID().c_str(), password))
    Serial.println("AP is up");
}

String getSSID()
{
  String stSsidWithMac = String(ssid);
  stSsidWithMac += "_" + WiFi.macAddress();
  stSsidWithMac.replace(":", "");
  Serial.println(stSsidWithMac);
  return stSsidWithMac;
}

StHtmlValues &getHtmlValues()
{
  return mglHtmlValues;
}

void setHtmlValues(StHtmlValues &newHtmlValues)
{
  mglHtmlValues = newHtmlValues;
}

void setupWifi()
{
  setupAccessPoint();
  setupServer();
}

void WifiTask()
{
  if (mglHtmlValues.bScanForWifi)
    scanForWifiNetworks();
}