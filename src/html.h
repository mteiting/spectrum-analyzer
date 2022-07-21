#pragma once

#include "Arduino.h"

const char *HtmlInput_Brightness = "input_brightness";
const char* PARAM_INPUT = "value";

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE html><html>
    <head><meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">

    <style>
      html {font-family: Arial; display: inline-block; text-align: center;}
      h3 {font-size: 1rem;}
      p {font-size: 1rem;}
      body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
      .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
        outline: none; -webkit-transition: .2s; transition: opacity .2s;}
      .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
      .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
    </style>

    <title>spectrum analyzer</title>
    </head>
    <body>

      <h1>Spectrum Analyzer</h1>
      <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
        .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
        text-decoration: none; font-size: 10px; margin: 2px; cursor: pointer;}
      </style>

      <h2>brightness</h2>
      <p><span id="textSliderValue">%input_brightness%</span></p>
      <p><input type="range" onchange="updateSliderBrightness(this)" id="brightnessSlider" min="0" max="100" value="%input_brightness%" step="1" class="slider"></p>
      <script>
        function updateSliderBrightness(element) {
          var sliderValue = document.getElementById("brightnessSlider").value;
          document.getElementById("textSliderValue").innerHTML = sliderValue;
          var xhr = new XMLHttpRequest();
          xhr.open("GET", "/slider?value="+sliderValue, true);
          xhr.send();
        }
      </script>
    
      <h2>peak LED delay [ms] </h2>
      PeakLedDelay: <input type="text" id="peakLedDelay" value="%input_peakleddelay%">
      <button onclick="changePeakDelay()">set peak led delay</button>
      <script>
      function changePeakDelay() {
        var delay = document.getElementById("peakLedDelay").value;
        document.getElementById("peakLedDelay").value = delay;
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/get_peakDelay?value="+delay, true);
        xhr.send();
      }
      </script>

      <h2> wifi scan </h2>
      <button onclick="scanWifi()">SCAN WIFI</button>
      <script>
      function scanWifi() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/scan_wifi");
        xhr.send();
      }
      </script>

      <select id = "wifiList">
        <option value="--">empty</option>
      </select>

      <script>
      setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            var list = document.getElementById("wifiList");
            while(list.length > 0)
            {
              list.remove(list.length-1);
            }
            let output = this.responseText.split("##");
            let currentWifi = 0;
            while(currentWifi < (output.length-1))
            {
              var option = document.createElement("option");
              option.value = output[currentWifi];
              option.text = output[currentWifi];
              currentWifi++;
              list.add(option); 
            }
          }
        };
        xhttp.open("GET", "/get_wifi", true);
        xhttp.send();
      }, 1000 ) ;
      </script>

    </body>
    </html>
)rawliteral";
