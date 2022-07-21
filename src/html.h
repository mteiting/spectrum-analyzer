#pragma once

#include "Arduino.h"

const char *HtmlInput_Brightness = "input_brightness";
const char *HtmlInput_PeakLedDelay = "input_peakleddelay";

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE html><html>
    <head><meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">

    <style>
      .dropbtn {
        background-color: #3498DB;
        color: white;
        padding: 16px;
        font-size: 16px;
        border: none;
        cursor: pointer;
      }
      .dropbtn:hover, .dropbtn:focus {
        background-color: #2980B9;
      }
      .dropdown {
        position: relative;
        display: inline-block;
      }
      .dropdown-content {
        display: none;
        position: absolute;
        background-color: #f1f1f1;
        min-width: 160px;
        overflow: auto;
        box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
        z-index: 1;
      }
      .dropdown-content a {
        color: black;
        padding: 12px 16px;
        text-decoration: none;
        display: block;
      }
      .dropdown a:hover {background-color: #ddd;}
      .show {display: block;}
    </style>

    <title>spectrum analyzer</title>
    </head>
    <body>
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
