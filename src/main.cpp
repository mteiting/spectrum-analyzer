#include <Arduino.h>
// #include <Wire.h>
#include <SparkFun_ADS1015_Arduino_Library.h>
#include <arduinoFFT.h>
#include <sstream>
#include <algorithm>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "led.h"
#include "band.h"
#include "analyzerWiFi.h"

#define SAMPLE_SIZE 512
#define SAMPLE_FREQUENCY 44100
#define BANDS 7
#define AMPLITUDE 1

ADS1015 microphone;

double vReal[SAMPLE_SIZE];
double vImag[SAMPLE_SIZE];
double maxvalue = 0;
arduinoFFT FFT = arduinoFFT();

int bands[] = {0, 0, 0, 0, 0, 0, 0};
int bands_normalized[] = {0, 0, 0, 0, 0, 0, 0};

bool bMicInitialized;

// void cont_adc_init()
// {
  
//   adc_digi_configuration_t cfg = {
//     .conv_limit_num = 512,
//     .sample_freq_hz = 44100,
//     .conv_mode = ADC_CONV_SINGLE_UNIT_1,
//     .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
//   };

// }

int get_frequency(int i)
{
  if (i < 2)
  {
    return 0;
  }
  return ((i - 2) * SAMPLE_FREQUENCY) / SAMPLE_SIZE;
}

void createBands(int i, int amplitude)
{
  //int frequency = get_frequency(i);
  uint8_t band = 0;

  if (i <= 4) // 63 Hz
  {
    band = 0;
  }
  else if (i <= 6) // 160 Hz
  {
    band = 1;
  }
  else if (i <= 7) // 400 Hz
  {
    band = 2;
  }
  else if (i <= 15) // 1 kHz
  {
    band = 3;
  }
  else if (i <= 32) // 2500 kHz
  {
    band = 4;
  }
  else if (i <= 75) // 6250 kHz
  {
    band = 5;
  }
  else
  {
    band = 6;
  }
  
  // if (frequency <= 125)
  // {
  //   band = 0;
  // }
  // else if (frequency <= 250)
  // {
  //   band = 1;
  // }
  // else if (frequency <= 500)
  // {
  //   band = 2;
  // }
  // else if (frequency <= 1000)
  // {
  //   band = 3;
  // }
  // else if (frequency <= 2000)
  // {
  //   band = 4;
  // }
  // else if (frequency <= 4000)
  // {
  //   band = 5;
  // }
  // else
  // {
  //   band = 6;
  // }

  bands[band] = amplitude;
}

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

void setup()
{
  Serial.begin(115200);

  // Wire.begin(-1, -1, 1000000UL);

  // bMicInitialized = microphone.begin();
  // if (bMicInitialized)
  // {
  //   Serial.println("Microphone found, I2C connected");
  // }
  // else
  // {
  //   Serial.println("No device found.");
  // }

  // setupWifi();

  // analyzer.setup();
  // Band *band = new Band(0, 0, 20, EnLedCountDir::enLedCountDir_Top);
  // Band *band2 = new Band(1, 20, 20, EnLedCountDir::enLedCountDir_Down);
  // Band *band3 = new Band(2, 40, 20, EnLedCountDir::enLedCountDir_Top);
  // Band *band4 = new Band(3, 60, 20, EnLedCountDir::enLedCountDir_Down);
  // Band *band5 = new Band(4, 80, 20, EnLedCountDir::enLedCountDir_Top);
  // Band *band6 = new Band(5, 100, 20, EnLedCountDir::enLedCountDir_Down);
  // Band *band7 = new Band(6, 120, 20, EnLedCountDir::enLedCountDir_Top);
  // analyzer.setBand(band);
  // analyzer.setBand(band2);
  // analyzer.setBand(band3);
  // analyzer.setBand(band4);
  // analyzer.setBand(band5);
  // analyzer.setBand(band6);
  // analyzer.setBand(band7);

  
}

void loop()
{
  //Serial.println(analogRead(4));
  // if (!bMicInitialized)
  // {
  //   Serial.println("Mic not initialized...");
  //   delay(1000);
  // }

  for (int i = 0; i < SAMPLE_SIZE; i++)
  {
    //vReal[i] = 4096 * cos(PI*2 * i * 1000/SAMPLE_FREQUENCY);    
    vReal[i] = static_cast<int16_t>(analogRead(32));//microphone.getSingleEnded(2);
    vImag[i] = 0.0;

    uint32_t start = micros();    
    while ((micros() - start) <= 6)
    {
      
    }
    
  }

  FFT.Windowing(vReal, SAMPLE_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLE_SIZE, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLE_SIZE);


  for (int i = 0; i < (SAMPLE_SIZE >> 1); i++)
  {
    double freq = (i * 1.0 * SAMPLE_FREQUENCY) / SAMPLE_SIZE;
    //Serial.printf("%4.2fHz : %4.2f\n", freq, vReal[i]);
  }
  
  int k = 0;
  memset(bands, 0, sizeof(int) * 7);
  for (int i = 2; i < (SAMPLE_SIZE >> 1); i++)
  {
    if (vReal[i] < 1000.0) {
      continue;
    }
    createBands(i, static_cast<int32_t>(vReal[i]));
  }

  for (int i = 0; i < BANDS; i++)
  {
    Serial.printf("%d : %4d\t", i, bands[i]);
  }
  Serial.println();

  // constexpr uint16_t REFRESH_RATE_MS = 200;

  // static auto offset = millis();
  // if ((millis() - offset) > REFRESH_RATE_MS)
  // {
  //   analyzer.loop(bands_normalized);
  //   offset = millis();
  // }

  // WifiTask();
}