#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "SpectrumAnalyzer";
const char* password = "1234567890";
ESP8266WebServer webServer(80);


void handleRoot() {
  webServer.send(200, "text/plain", "hello from esp8266!");
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
  WiFi.mode(WIFI_AP);
  if(true == WiFi.softAP(ssid, password))
    Serial.println("AP is up");
}

void setupWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
}

void setupServerContent()
{
  webServer.on("/", handleRoot);
  webServer.on("/inline", [](){
    webServer.send(200, "text/plain", "this works as well");
  });

  webServer.onNotFound(handleNotFound);
  webServer.begin();
  Serial.println("HTTP server started");
}



void setup(void){
  Serial.begin(115200);
  system_update_cpu_freq(160);
  setupAccessPoint();
  setupServerContent();
}

void loop(void){
  webServer.handleClient();
}
