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
#include "tools.h"

const char *ssid = "SpectrumAnalyzer";
const char *password = "1234567890";

static AsyncWebServer server(80);
static StHtmlValues mglHtmlValues = {};
static std::string mglsWifiList = "";

static void scanForWifiNetworks()
{
  mglHtmlValues.bScanForWifi = false;
  std::vector<std::string> vSsids;

  int16_t i16NumOfWifiNetworks = WiFi.scanNetworks();

  for (uint8_t u8CurrentWifi = 0; u8CurrentWifi < static_cast<uint8_t>(i16NumOfWifiNetworks); u8CurrentWifi++)
  {
    std::string newWifi = std::string(WiFi.SSID(u8CurrentWifi).c_str());
    if (std::find(vSsids.begin(), vSsids.end(), newWifi) != vSsids.end())
      continue;

    vSsids.push_back(newWifi);
    mglsWifiList.append(newWifi);
    mglsWifiList += "##";
  }
}

static void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "NOT FOUND");
}

static void badRequest(AsyncWebServerRequest *request)
{
  request->send(400, "text/plain", "BAD REQUEST");
}

static void ok(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain", "OK");
}

static String sliderBrightnessValue(const String &var)
{
  if (var.equals(HtmlInput_Brightness))
    return String(mglHtmlValues.u8Brightness);
  return String();
}

static void setupServer()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  server.on("/peakDelay", HTTP_POST, [](AsyncWebServerRequest *request)
            {
            if (!request->hasParam(PARAM_INPUT)){
              badRequest(request);
            } else {
              mglHtmlValues.u32PeakLedDelay = request->getParam(PARAM_INPUT)->value().toInt();
              FileHandlingSaveHtml(mglHtmlValues);
              ok(request);  
            } });

  server.on("/micGain", HTTP_POST, [](AsyncWebServerRequest *request)
            {
            if (!request->hasParam(PARAM_INPUT)){
              badRequest(request);
            } else {
              mglHtmlValues.fGain = DEFAULT_GAIN + (request->getParam(PARAM_INPUT)->value().toFloat() / 100);
              FileHandlingSaveHtml(mglHtmlValues);
              ok(request);
            } });

  server.on("/colorFadeOffset", HTTP_POST, [](AsyncWebServerRequest *request)
            {
            if (!request->hasParam(PARAM_INPUT)){
              badRequest(request);
            } else {
              mglHtmlValues.dColorFadeOffset = request->getParam(PARAM_INPUT)->value().toFloat();
              FileHandlingSaveHtml(mglHtmlValues);
              ok(request);  
            } });

  server.on("/all_toggle", HTTP_POST, [](AsyncWebServerRequest *request)
            { 
            mglHtmlValues.bAllToggle = !mglHtmlValues.bAllToggle;
            mglHtmlValues.bLedTestStart = false;
            mglHtmlValues.bSimulationStart = false;
            ok(request); });

  server.on("/fading_toggle", HTTP_POST, [](AsyncWebServerRequest *request)
            { 
            mglHtmlValues.bFading = !mglHtmlValues.bFading;
            FileHandlingSaveHtml(mglHtmlValues);
            ok(request); });

  server.on("/test_start", HTTP_POST, [](AsyncWebServerRequest *request)
            { 
            mglHtmlValues.bLedTestStart = !mglHtmlValues.bLedTestStart;
            mglHtmlValues.bAllToggle = false;
            mglHtmlValues.bSimulationStart = false;
            ok(request); });

  server.on("/sim_start", HTTP_POST, [](AsyncWebServerRequest *request)
            { 
            mglHtmlValues.bSimulationStart = !mglHtmlValues.bSimulationStart;
            mglHtmlValues.bAllToggle = false;
            mglHtmlValues.bLedTestStart = false;
            ok(request); });

  server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request)
            { 
            mglHtmlValues.bScanForWifi = true;
            ok(request); });

  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
            if(mglsWifiList.empty()){
              request->send(204, "text/plain", ""); 
            } else {
              request->send(200, "text/plain", mglsWifiList.c_str()); 
              mglsWifiList = "";
            } });

  server.on("/brightness", HTTP_POST, [](AsyncWebServerRequest *request)
            {
            if (!request->hasParam(PARAM_INPUT)) {
              badRequest(request);
            } else {
              String sliderValue = request->getParam(PARAM_INPUT)->value();
              mglHtmlValues.u8Brightness = sliderValue.toInt();
              FileHandlingSaveHtml(mglHtmlValues);
              ok(request);
            } });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
            String response = String(mglHtmlValues.u8Brightness) + "\n";
            response += String((mglHtmlValues.fGain - DEFAULT_GAIN) * 100, 0) + "\n";
            response += String(mglHtmlValues.u32PeakLedDelay)+ "\n";
            response += String(mglHtmlValues.dColorFadeOffset, 0);
            request->send(200, "text/plain", response.c_str()); });

  server.on("/wifiConnect", HTTP_POST, [](AsyncWebServerRequest *request)
            {
            if (!request->hasParam("ssid") || !request->hasParam("password")) {
              request->send(400, "text/plain", "NETWORK OR PASSWORD MISSING"); 
            } else {
              String password = request->getParam("password")->value();
              String ssid = request->getParam("ssid")->value();
              mglHtmlValues.wifiPW = password;
              mglHtmlValues.wifiSSID = ssid;
              mglHtmlValues.bReconnectWifi = true;
              FileHandlingSaveHtml(mglHtmlValues);
              ok(request);
            } });

  server.onNotFound(notFound);
  server.begin();
}

static void setupAccessPoint()
{
  Serial.println("\ninit AP mode ...");
  WiFi.mode(WIFI_AP_STA);
  if (true == WiFi.softAP(getSSID().c_str(), password))
  {
    Serial.println("AP is up");
  }
}

static wl_status_t setupStation()
{
  uint32_t u32TimerWlanConnet = 0;
  Serial.println("init Station mode ...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(
      mglHtmlValues.wifiSSID.c_str(),
      mglHtmlValues.wifiPW.c_str());
  Serial.printf("connecting to %s\n", mglHtmlValues.wifiSSID.c_str());

  while (WiFi.status() != WL_CONNECTED)
  {
    if (isTimeExpired(u32TimerWlanConnet, 10000))
    {
      WiFi.mode(WIFI_OFF);
      return WL_CONNECT_FAILED;
    }
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
  return WL_CONNECTED;
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
  if (mglHtmlValues.wifiSSID.isEmpty())
    setupAccessPoint();
  else if (WL_CONNECTED != setupStation())
    setupAccessPoint(); // do ap mode, if connection could not be established

  setupServer();
}

void WifiTask()
{
  if (mglHtmlValues.bScanForWifi)
    scanForWifiNetworks();

  if (mglHtmlValues.bReconnectWifi)
  {
    mglHtmlValues.bReconnectWifi = false;
    esp_restart(); // reboot tut gut
  }
}