#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "SpectrumAnalyzer";
const char* password = "1234567890";
ESP8266WebServer webServer(80);


String getSSID()
{
  String stSsidWithMac = String(ssid);
  stSsidWithMac += "_" + WiFi.macAddress();
  stSsidWithMac.replace(":", "");
  Serial.println(stSsidWithMac);
  return stSsidWithMac;
}

void handle_OnConnect() {
  // webServer.send(200, "text/plain", "hello from esp8266!");
  // Webseiten-Überschrift
  String stHtmlContent = String("<!DOCTYPE html><html>");
  stHtmlContent += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  stHtmlContent +=("<link rel=\"icon\" href=\"data:,\">");
  stHtmlContent +=("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  stHtmlContent +=(".button { background-color: #333344; border: none; color: white; padding: 16px 40px;");
  stHtmlContent +=("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  stHtmlContent +=(".button2 {background-color: #888899;}</style></head>");
  stHtmlContent +=("<body><h1>ESP8266 Web Server</h1>");
  stHtmlContent +=("</body></html>");
  stHtmlContent +=("");
  webServer.send(200, "text/plain", stHtmlContent);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webServer.uri();
  message += "\nMethod: ";
  message += (webServer.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += webServer.args();
  message += "\n";
  for (uint8_t i=0; i<webServer.args(); i++){
    message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
  }
  webServer.send(404, "text/plain", message);
}

void setupAccessPoint()
{
  Serial.println("init AP ...");
  WiFi.mode(WIFI_AP);
  if(true == WiFi.softAP(getSSID().c_str(), password))
    Serial.println("AP is up");
}

void setupWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(getSSID().c_str(), password);

  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(getSSID().c_str());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
}

void setupServerContent()
{
  webServer.on("/", handle_OnConnect);
  webServer.on("/inline", [](){
    webServer.send(200, "text/plain", "this works as well");
  });

  webServer.onNotFound(handleNotFound);
  Serial.println("HTTP server started");
  webServer.begin();
}

void setup(void){
  Serial.begin(115200);
  system_update_cpu_freq(160);
  delay(1000);
  while(!Serial);
  setupAccessPoint();
  setupServerContent();
}

void loop(void){
  webServer.handleClient();
}
                      