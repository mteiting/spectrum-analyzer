#pragma once

#include "Arduino.h"

const char *HtmlInput_Brightness = "input_brightness";
const char *PARAM_INPUT = "value";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="icon" href="data:," />

    <style>
      html {
        font-family: Arial;
        display: inline-block;
        text-align: center;
      }
      h3 {
        font-size: 1rem;
      }
      p {
        font-size: 1rem;
      }
      body {
        max-width: 400px;
        margin: 0px auto;
        padding-bottom: 25px;
      }
      .slider {
        margin: 14px;
        width: 360px;
        height: 25px;
        background: #ffd65c;
        outline: none;
        -webkit-transition: 0.2s;
        transition: opacity 0.2s;
      }
      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 35px;
        height: 35px;
        background: #003249;
        cursor: pointer;
      }
      .slider::-moz-range-thumb {
        width: 35px;
        height: 35px;
        background: #003249;
        cursor: pointer;
      }
      .input {
        display: block;
        margin: 1rem;
      }
    </style>

    <title>Spectrum Analyzer</title>
  </head>
  <body>
    <h1>Spectrum Analyzer</h1>
    <style>
      html {
        font-family: Helvetica;
        display: inline-block;
        margin: 10px auto;
        text-align: center;
      }
    </style>

    <h2>Brightness</h2>
    <div>
      <p><span id="brightnessText">0</span></p>
      <input
        type="range"
        onchange="updateBrightness()"
        id="brightnessSlider"
        min="0"
        max="100"
        step="1"
        class="slider"
      />
    </div>

    <h2>Peak LED delay [ms]</h2>
    <div>
      <div class="input">
        <input type="text" id="peakLedDelay" value="0" />
      </div>
      <div class="button">
        <button onclick="updatePeakDelay()">Set delay</button>
      </div>
    </div>

    <h2>Mic gain [%]</h2>
    <div>
      <p><span id="micGainText">0</span></p>
      <div class="input">
        <input
          type="range"
          id="micGainSlider"
          value="0"
          onchange="updateMicGain()"
          min="-100"
          max="100"
          step="5"
          class="slider"
        />
      </div>
    </div>
    
    <h2>Color Offset</h2>
    <div>
      <p><span id="colorOffsetText">0</span></p>
      <div class="input">
        <input
          type="range"
          id="colorOffsetSlider"
          value="0"
          onchange="updateColorOffset()"
          min="0"
          max="500"
          step="1"
          class="slider"
        />
      </div>
    </div>
    
    <h2>Fading</h2>
    <div>
      <button onclick="fadingToggle()">toggle fading</button>
    </div>

    <h2>All toggle</h2>
    <div>
      <button onclick="toggleAll()">Toggle toggleAll</button>
    </div>

    <h2>Testing</h2>
    <div>
      <button onclick="testStart()">Start LED Test</button>
    </div>

    <h2>Simulation</h2>
    <div>
      <button onclick="simStart()">Start Simulation</button>
    </div>

    <h2>WIFI Scan</h2>
    <div>
      <button onclick="scanWifi()">Scan for WIFI</button>
    </div>

    <div style="margin: 1rem">
      <div style="min-width: 200px">
        <select style="min-width: 200px" id="wifiList"></select>
      </div>

      <div class="input">
        <label for="wifiPasswordInput">Password:</label>
        <input id="wifiPasswordInput" type="password" />
        <button
          id="wifiPasswordToggleButton"
          onclick="togglePasswordVisibility()"
        >
          Show
        </button>
      </div>
    </div>

    <div>
      <button id="connectToWifi" onclick="connectToWifi()">
        Connect
      </button>
    </div>

    <script>
      function sendRequest(method, url, callback) {
        console.log(method, url);
        let xhr = new XMLHttpRequest();
        xhr.onreadystatechange = (event) => {
          if (callback && xhr.readyState == xhr.DONE) {
            callback(xhr.responseText);
          }
        };
        xhr.open(method, url, true);
        xhr.send();
      }

      function updateBrightness() {
        let brightnessSlider = document.getElementById("brightnessSlider");
        sendRequest(
          "POST",
          `/brightness?value=${brightnessSlider.value}`,
          console.log
        );
      }

      function updatePeakDelay() {
        let peakDelayInput = document.getElementById("peakLedDelay");
        sendRequest(
          "POST",
          `/peakDelay?value=${peakLedDelay.value}`,
          console.log
        );
      }

      function updateColorOffset() {
        let colorFadeOffset= document.getElementById("colorOffsetSlider");
        sendRequest("POST", `/colorFadeOffset?value=${colorFadeOffset.value}`, console.log);
      }

      function updateMicGain() {
        let micGain = document.getElementById("micGainSlider");
        sendRequest("POST", `/micGain?value=${micGain.value}`, console.log);
      }

      function scanWifi() {
        sendRequest("POST", "/wifi", () => {
          setTimeout(getWifiList(), 2000);
        });
      }

      function toggleAll() {
        sendRequest("POST", "/all_toggle", null);
      }

      function simStart() {
        sendRequest("POST", "/sim_start", null);
      }

      function testStart() {
        sendRequest("POST", "/test_start", null);
      }

      function fadingToggle() {
        sendRequest("POST", "/fading_toggle", null);
      }

      function propagate_wifi_list(response) {
        let wifiList = document.getElementById("wifiList");

        if (response.length == 0) {
          return;
        }

        for (let i = 0; i < wifiList.length; i++) {
          wifiList.remove(wifiList.length - 1);
        }

        let wifiNetworks = response.split("##");
        if (wifiNetworks.length == 0) {
          return;
        }

        for (let i = 0; i < wifiNetworks.length; i++) {
          if (wifiNetworks[i].length == 0) {
            continue;
          }

          let newOption = document.createElement("option");
          newOption.value = wifiNetworks[i];
          newOption.text = wifiNetworks[i];
          wifiList.add(newOption);
        }
      }

      function getWifiList() {
        sendRequest("GET", "/wifi", propagate_wifi_list);
      }

      function togglePasswordVisibility() {
        let passwordInput = document.getElementById("wifiPasswordInput");
        let toggleButton = document.getElementById("wifiPasswordToggleButton");

        if (passwordInput.type === "password") {
          passwordInput.type = "text";
          toggleButton.textContent = "Hide";
        } else {
          passwordInput.type = "password";
          toggleButton.textContent = "Show";
        }
      }

      function connectToWifi() {
        let wifiList = document.getElementById("wifiList");
        let passwordInput = document.getElementById("wifiPasswordInput");
        let selectedOptions = wifiList.options;
        let selectedIndex = selectedOptions.selectedIndex;

        let selectedOption = selectedOptions[selectedIndex].value;
        let password = passwordInput.value;

        sendRequest(
          "POST",
          `/wifiConnect?ssid=${selectedOption}&password=${password}`,
          console.log
        );
      }

      setInterval(getWifiList, 5000);

      const initialize_page = () => {
        sendRequest("GET", "/settings", (response) => {
          let brightnessText = document.getElementById("brightnessText");
          let brightnessSlider = document.getElementById("brightnessSlider");
          let peakDelayInput = document.getElementById("peakLedDelay");
          let micGainSlider = document.getElementById("micGainSlider");
          let micGainText = document.getElementById("micGainText");
          let colorOffsetSlider = document.getElementById("colorOffsetSlider");
          let colorOffsetText = document.getElementById("colorOffsetText");

          if (response === "") {
            return;
          }

          let values = response.split(/\r?\n/);
          brightnessSlider.value = values[0];
          brightnessText.textContent = values[0];
          micGainSlider.value = values[1];
          micGainText.textContent = values[1];
          peakDelayInput.value = values[2];
          colorOffsetSlider.value = values[3];
          colorOffsetText.textContent = values[3];
        });
      };

      window.onload = function () {
        let brightnessText = document.getElementById("brightnessText");
        let brightnessSlider = document.getElementById("brightnessSlider");
        brightnessSlider.addEventListener("input", (event) => {
          brightnessText.textContent = event.target.value;
        });

        let micGainSlider = document.getElementById("micGainSlider");
        let micGainText = document.getElementById("micGainText");
        micGainSlider.addEventListener("input", (event) => {
          micGainText.textContent = event.target.value;
        });

        let colorOffsetSlider = document.getElementById("colorOffsetSlider");
        let colorOffsetText = document.getElementById("colorOffsetText");
        colorOffsetSlider.addEventListener("input", (event) => {
          colorOffsetText.textContent = event.target.value;
        });

        initialize_page();
      };
    </script>
  </body>
</html>

)rawliteral";
