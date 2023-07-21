#include "analyzerFFT.h"
#include <arduinoFFT.h>
#include <algorithm>
#include "driver/i2s.h"
#include "esp_event.h"
#include "analyzerWiFi.h"
#include "defaults.h"

static arduinoFFT FFT = arduinoFFT();
static QueueHandle_t i2s_event_queue;
static uint32_t bands[] = {0, 0, 0, 0, 0, 0, 0};
static std::vector<uint8_t> normalized_bands(BANDS, 0);

static uint32_t get_frequency(int i)
{
  if (i < 2)
    return 0;

  return ((i - 2) * SAMPLE_FREQUENCY) / SAMPLE_SIZE;
}

static void createBands(int i, uint32_t amplitude)
{
  uint32_t frequency = get_frequency(i);
  uint8_t band = 0;
  amplitude = amplitude * getHtmlValues().fGain;

  if (frequency <= 63)
  {
    band = 0;
  }
  else if (frequency <= 160)
  {
    band = 1;
  }
  else if (frequency <= 400)
  {
    band = 2;
  }
  else if (frequency <= 1000)
  {
    band = 3;
  }
  else if (frequency <= 2500)
  {
    band = 4;
  }
  else if (frequency <= 6250)
  {
    band = 5;
    amplitude *= 1.50;
  }
  else
  {
    band = 6;
    amplitude *= 2;
  }
  bands[band] = std::max(bands[band], amplitude);
}

std::vector<uint8_t> &getBandsFromFFT()
{
  return normalized_bands;
}

void analyzerFFT_Setup()
{
  i2s_config_t i2s_config;
  i2s_config.mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN);
  i2s_config.sample_rate = SAMPLE_FREQUENCY;              // 120 KHz
  i2s_config.dma_buf_len = SAMPLE_SIZE;                   // 512
  i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT; // Should be mono but doesn't seem to be
  i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  i2s_config.use_apll = false,
  i2s_config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  i2s_config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  i2s_config.dma_buf_count = 2;
  // install and start i2s driver
  i2s_driver_install(I2S_NUM_0, &i2s_config, 1, &i2s_event_queue);
  // Connect ADC to I2S
  i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_6);
  i2s_adc_enable(I2S_NUM_0);
}

void analyzerFFT_Task()
{
  static double vReal[SAMPLE_SIZE];
  static double vImag[SAMPLE_SIZE];
  uint16_t i2s_read_buff[SAMPLE_SIZE];
  size_t bytesRead = 0;
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

      FFT.Windowing(vReal, SAMPLE_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLE_SIZE, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLE_SIZE);

      memset(bands, 0, sizeof(int) * 7);
      for (int i = 2; i < (SAMPLE_SIZE >> 1); i++)
      {
        if (vReal[i] < 2000.0)
          continue;

        createBands(i, static_cast<uint32_t>(vReal[i]));
      }

      for (int i = 0; i < BANDS; i++)
      {
        uint8_t value = static_cast<uint8_t>((bands[i] * 100) / 160000);
        if (value > 100)
          value = 100;
        normalized_bands[i] = value;
      }
    }
  }
}