#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <map>
#include <iterator>

const char *ssid = "SpectrumAnalyzer";
const char *password = "1234567890";

static WiFiServer server(80);
static std::map<IPAddress, WiFiClient *> clients;

void getHTMLPage(WiFiClient &currentClient)
{
  Serial.println("Add Client");
  clients.insert(std::make_pair(currentClient.localIP(), &currentClient));

  currentClient.println("HTTP/1.1 200 OK");
  currentClient.println("Content-type:text/html");
  currentClient.println("Connection: close");
  currentClient.println();
  //HTML content
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
}

void loop()
{
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