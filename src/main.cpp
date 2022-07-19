#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_ADS1015_Arduino_Library.h>
#include <arduinoFFT.h>
#include <sstream>
#include <iomanip>

#define SAMPLE_SIZE 128
#define SAMPLE_FREQUENCY 16000
#define BANDS 7

ADS1015 microphone;

double vReal[SAMPLE_SIZE];
double vImag[SAMPLE_SIZE];
double maxvalue = 0;
arduinoFFT FFT = arduinoFFT();

int bands[] = { 0, 0, 0, 0, 0, 0, 0 };
int bands_normalized[] = { 0, 0, 0, 0, 0, 0, 0 };

bool bMicInitialized;

int get_frequency(int i) 
{
  if (i < 2) {
    return 0;
  }
  return (i - 2) * (SAMPLE_FREQUENCY / SAMPLE_SIZE );
}

void createBands(int i, int amplitude)
{
  int frequency = get_frequency(i);
  uint8_t band = 0;

  Serial.printf("%2d: %2d\n", i, frequency);

  if (frequency <= 125)
  {
    band = 0;
  }
  else if (frequency <= 250)
  {
    band = 1;
  }
  else if (frequency <= 500)
  {
    band = 2;
  }
  else if (frequency <= 1000)
  {
    band = 3;
  }
  else if (frequency <= 2000)
  {
    band = 4;
  }
  else if (frequency <= 4000)
  {
    band = 5;
  }
  else if (frequency <= 8000)
  {
    band = 6;
  }
  else
  {
    band = 7;
  }

  int max_amplitude = 2048; // amplitude;
  if (amplitude > max_amplitude)
  {
    amplitude = max_amplitude;
  }
  if (amplitude > bands[band])
  {
    bands[band] = amplitude;
  }
}

void setup() {  
  Serial.begin(115200);
  Wire.begin();

  bMicInitialized = microphone.begin();
  if (bMicInitialized)
  {
    Serial.println("Microphone found, I2C connected");
  }
  else
  {
    Serial.println("No device found.");
  }
}


void loop() {
    if (!bMicInitialized)
    {
      Serial.println("Mic not initialized...");
      delay(1000);
    }

    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
      vReal[i] = (double) microphone.getSingleEnded(2); 
      vImag[i] = 0.0;
    }    

    FFT.Windowing(vReal, SAMPLE_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLE_SIZE);
    double x = FFT.MajorPeak(vReal, SAMPLE_SIZE, SAMPLE_FREQUENCY);
    
    int k = 0;
    memset(bands, 0, sizeof(int) * 7);
    std::stringstream result;
    for (int i = 2; i < SAMPLE_SIZE; i++)
    { 
      createBands(i, vReal[i]);
        //maxvalue = maxvalue < vReal[i] ? vReal[i] : maxvalue;
        //result << std::setw(2) << k++ << ": ";
        //result << std::setprecision(2) << std::fixed << std::setw(5) << vReal[i] << " ; ";        
    }

    for (int i = 0; i < BANDS; i++)
    {
      bands_normalized[i] = bands[i] * 100 / 2048;
      //Serial.printf("%d : %d\t", i+1, bands_normalized[i]);
    }
    Serial.println();
    
    //Serial.println(maxvalue);
    //Serial.println(result.str().c_str());
    
    //delay(100);
}