#pragma once

#include "Arduino.h"

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE html><html>
    <head><meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <title>spectrum analyzer</title></head><body>
    <h1>Spectrum Analyzer</h1>
    <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
      .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
      text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
    </style></head>
    
    <form action="/get">
      Brightness: <input type="text" name="input_brightness">
     <input type="submit" value="Submit">
    </form><br>
    <form action="/get">
      PeakLedDelay: <input type="text" name="input_peakleddelay">
      <input type="submit" value="Submit">
    </form><br>

    <button onclick="myFunction()">SCAN WIFI</button>
    <script>
    function myFunction() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/scan_wifi");
      xhr.send();
    }
    </script>

    </body></html>
)rawliteral";
