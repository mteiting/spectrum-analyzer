#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "led.h"
#include "band.h"

  // Declare our NeoPixel strip object:
  static Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
  // Argument 1 = Number of pixels in NeoPixel strip
  // Argument 2 = Arduino pin number (most are valid)
  // Argument 3 = Pixel type flags, add together as needed:
  //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
  //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
  //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
  //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
  //   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
  static Analyzer analyzer(&strip);



const char *ssid = "SpectrumAnalyzer";
const char *password = "1234567890";

static WiFiServer server(80);

void getHTMLPage(WiFiClient &currentClient)
{
  Serial.println("Add Client");
  // clients.insert(std::make_pair(currentClient.localIP(), &currentClient));

  currentClient.println("HTTP/1.1 200 OK");
  currentClient.println("Content-type:text/html");
  currentClient.println("Connection: close");
  currentClient.println();
  // HTML content
  currentClient.println("<!DOCTYPE html><html>");
  currentClient.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  currentClient.println("<link rel=\"icon\" href=\"data:,\">");
  // web page heading inside browser
  currentClient.println("<title>spectrum analyzer</title></head><body>");
  // topic
  currentClient.println("<h1>Spectrum Analyzer</h1>");
  // Scan Button
  currentClient.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  currentClient.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  currentClient.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  currentClient.println("</style></head>");
  currentClient.println("<p><a <button class=\"button\">SCAN</button></a></p>");
  // end of HTML page
  currentClient.println("</body></html>");
  currentClient.println();
}

String getSSID()
{
  String stSsidWithMac = String(ssid);
  stSsidWithMac += "_" + WiFi.macAddress();
  stSsidWithMac.replace(":", "");
  Serial.println(stSsidWithMac);
  return stSsidWithMac;
}

void setupServer()
{
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
}

void setupAccessPoint()
{
  Serial.println("init AP ...");
  WiFi.mode(WIFI_AP);
  if (true == WiFi.softAP(getSSID().c_str(), password))
    Serial.println("AP is up");
}

void setupWifi()
{
  setupAccessPoint();
}

void setup()
{
  Serial.begin(115200);
  setupWifi();
  setupServer();

  analyzer.setup();
  Band *band = new Band(0, 0, 20000, 20);
  Band *band2 = new Band(20, 0, 20000, 20);
  Band *band3 = new Band(40, 0, 20000, 20);
  Band *band4 = new Band(60, 0, 20000, 20);
  Band *band5 = new Band(80, 0, 20000, 20);
  analyzer.setBand(band);
  analyzer.setBand(band2);
  analyzer.setBand(band3);
  analyzer.setBand(band4);
  analyzer.setBand(band5);
}

void loop()
{
  analyzer.loop();

  WiFiClient client = server.available();
  if (!client)
    return;

  if (client.available())
  {
    getHTMLPage(client);
  }
  else
  {
    client.stop();
  }
}