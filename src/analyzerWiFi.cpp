#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "AsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include "analyzerWiFi.h"
#include "html.h"

const char *ssid = "SpectrumAnalyzer";
const char *password = "1234567890";

static AsyncWebServer server(80);
static StHtmlValues mglHtmlValues = {};
static std::string mglsWifiList = "";

static void scanForWifiNetworks()
{
  Serial.println("Scanning...");
  mglHtmlValues.bScanForWifi = false;

  uint16_t u16NumOfWifiNetworks = WiFi.scanNetworks();
  Serial.println(u16NumOfWifiNetworks);
  for (uint8_t u8CurrentWifi = 0; u8CurrentWifi < u16NumOfWifiNetworks; u8CurrentWifi++)
  {
    std::string newWifi = std::string(WiFi.SSID(u8CurrentWifi).c_str());
    mglsWifiList.append(newWifi);
    mglsWifiList += "##";
  }
  Serial.println(mglsWifiList.c_str());
}

static void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

static void setupServer()
{
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
            if (request->hasParam(HtmlInput_Brightness)) {
              mglHtmlValues.u8Brightness = request->getParam(HtmlInput_Brightness)->value().toInt();
            }
            else if (request->hasParam(HtmlInput_PeakLedDelay)) {
              mglHtmlValues.u8PeakLedDelay = request->getParam(HtmlInput_PeakLedDelay)->value().toInt();
            }
            else {
            }
            // request->send(200, "text/html", "<br><a href=\"/\">Return to Home Page</a>"); 
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
            Serial.println("sending wifi");
            request->send(200, "text/plain", mglsWifiList.c_str()); 
            mglsWifiList.erase(); });

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