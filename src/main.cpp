#include <Arduino.h>
#include <SparkFun_ADS1015_Arduino_Library.h>
#include <arduinoFFT.h>
#include <sstream>
#include <algorithm>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "led.h"
#include "band.h"
#include "analyzerWiFi.h"
#include "driver/i2s.h"
#include "esp_event.h"


#define SAMPLE_SIZE 512ul
#define SAMPLE_FREQUENCY 44000ul
#define BANDS 7
#define AMPLITUDE 1

ADS1015 microphone;

double vReal[SAMPLE_SIZE];
double vImag[SAMPLE_SIZE];
double maxvalue = 0;
arduinoFFT FFT = arduinoFFT();
QueueHandle_t i2s_event_queue;

uint32_t bands[] = {0, 0, 0, 0, 0, 0, 0};
int bands_normalized[] = {0, 0, 0, 0, 0, 0, 0};

bool bMicInitialized;

i2s_config_t i2s_config;


uint32_t get_frequency(int i)
{
  if (i < 2)
  {
    return 0;
  }
  return ((i - 2) * SAMPLE_FREQUENCY) / SAMPLE_SIZE;
}

void createBands(int i, uint32_t amplitude)
{
  uint32_t frequency = get_frequency(i);
  uint8_t band = 0;

  if (frequency <= 63) // (i <= 4) // 63 Hz
  {
    band = 0;
  }
  else if (frequency <= 160) //(i <= 6) // 160 Hz
  {
    band = 1;
  }
  else if (frequency <= 400) //(i <= 7) // 400 Hz
  {
    band = 2;
  }
  else if (frequency <= 1000) //(i <= 15) // 1 kHz
  {
    band = 3;
  }
  else if (frequency <= 2500) //(i <= 32) // 2500 kHz
  {
    band = 4;
  }
  else if (frequency <= 6250) // (i <= 75) // 6250 kHz
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

  bands[band] = std::max(bands[band], amplitude);
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

  i2s_config.mode =  i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN);
  i2s_config.sample_rate = SAMPLE_FREQUENCY;                           // 120 KHz
  i2s_config.dma_buf_len = SAMPLE_SIZE;                           // 512
  i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;         // Should be mono but doesn't seem to be
  i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  i2s_config.use_apll = false,
  i2s_config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  i2s_config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  i2s_config.dma_buf_count = 2;
  //install and start i2s driver
  i2s_driver_install(I2S_NUM_0, &i2s_config, 1, &i2s_event_queue);
  // Connect ADC to I2S
  i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_6);
  i2s_adc_enable(I2S_NUM_0);


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

  setupWifi();

  analyzer.setup();
  Band *band = new Band(0, 0, 20, EnLedCountDir::enLedCountDir_Top);
  Band *band2 = new Band(1, 20, 20, EnLedCountDir::enLedCountDir_Down);
  Band *band3 = new Band(2, 40, 20, EnLedCountDir::enLedCountDir_Top);
  Band *band4 = new Band(3, 60, 20, EnLedCountDir::enLedCountDir_Down);
  Band *band5 = new Band(4, 80, 20, EnLedCountDir::enLedCountDir_Top);
  Band *band6 = new Band(5, 100, 20, EnLedCountDir::enLedCountDir_Down);
  Band *band7 = new Band(6, 120, 20, EnLedCountDir::enLedCountDir_Top);
  analyzer.setBand(band);
  analyzer.setBand(band2);
  analyzer.setBand(band3);
  analyzer.setBand(band4);
  analyzer.setBand(band5);
  analyzer.setBand(band6);
  analyzer.setBand(band7);

  
}

void loop()
{
  //Serial.println(analogRead(4));
  // if (!bMicInitialized)
  // {
  //   Serial.println("Mic not initialized...");
  //   delay(1000);
  // }

  size_t bytesRead = 0;
  uint16_t i2s_read_buff[SAMPLE_SIZE];
  system_event_t evt;
  if (xQueueReceive(i2s_event_queue, &evt, portMAX_DELAY) == pdPASS)
  {
    if (evt.event_id == 2)
    {
      i2s_read(I2S_NUM_0, (char *)i2s_read_buff, SAMPLE_SIZE * 2, &bytesRead, portMAX_DELAY);
      for (int i = 0; i < SAMPLE_SIZE; i++)
      {
        vReal[i] = static_cast<double>(i2s_read_buff[i] & 0xfff); 
        vImag[i] = 0.0;
      }
    }
  }

  FFT.Windowing(vReal, SAMPLE_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLE_SIZE, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLE_SIZE);

  // for (int i = 2; i < (SAMPLE_SIZE >> 1); i++)
  // {
  //   double freq = (i * 1.0 * SAMPLE_FREQUENCY) / SAMPLE_SIZE;
  //   Serial.printf("%4.2fHz : %4.2f\n", freq, vReal[i]);
  // }  
  
  memset(bands, 0, sizeof(int) * 7);
  for (int i = 2; i < (SAMPLE_SIZE >> 1); i++)
  {
    if (vReal[i] < 2000.0) {
      continue;
    }
    createBands(i, static_cast<uint32_t>(vReal[i]));
  }

  for (int i = 0; i < BANDS; i++)
  {
    Serial.printf("%d : %4d\t", i, bands[i]);
    bands_normalized[i] = (bands[i] * 100) / 300000;
  }
  Serial.println();

  analyzer.loop(bands_normalized);
}