#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "AsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include "analyzerWiFi.h"
#include "html.h"

const char *ssid = "SpectrumAnalyzer";
const char *password = "1234567890";

static AsyncWebServer server(80);
static StHtmlValues mglHtmlValues = {};
const char *HtmlInput_Brightness = "input_brightness";
const char *HtmlInput_PeakLedDelay = "input_peakleddelay";

static void scanForWifiNetworks()
{
  Serial.println("Scanning...");
  mglHtmlValues.bScanForWifi = false;

  uint16_t u16NumOfWifiNetworks = WiFi.scanNetworks();
  Serial.println(u16NumOfWifiNetworks);
  for (int i = 0; i < u16NumOfWifiNetworks; ++i)
  {
    // Print SSID and RSSI for each network found
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(")");
    Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
    delay(10);
  }
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