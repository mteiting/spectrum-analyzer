#include <Arduino.h>
#include <memory>
#include "Adafruit_NeoPixel.h"
#include "Analyzer.h"
#include "analyzerFFT.h"
#include "led.h"
#include "band.h"
#include "analyzerWiFi.h"
#include "tools.h"
#include "FileHandling.h"

std::shared_ptr<Analyzer> _analyzer = nullptr;

void setup()
{
  Serial.begin(115200);

  FileHandlingSetup();
  analyzerFFT_Setup();
  setupWifi();

  std::shared_ptr<Adafruit_NeoPixel> _strip1 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_1_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip2 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_2_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip3 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_3_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip4 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_4_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip5 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_5_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip6 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_6_PIN, NEO_GRB + NEO_KHZ800);
  std::shared_ptr<Adafruit_NeoPixel> _strip7 = std::make_shared<Adafruit_NeoPixel>(LED_COUNT, BAND_7_PIN, NEO_GRB + NEO_KHZ800);

  std::shared_ptr<Band> band1 = std::make_shared<Band>(_strip1, 0, 0, 20);
  std::shared_ptr<Band> band2 = std::make_shared<Band>(_strip2, 1, 0, 20);
  std::shared_ptr<Band> band3 = std::make_shared<Band>(_strip3, 2, 0, 20);
  std::shared_ptr<Band> band4 = std::make_shared<Band>(_strip4, 3, 0, 20);
  std::shared_ptr<Band> band5 = std::make_shared<Band>(_strip5, 4, 0, 20);
  std::shared_ptr<Band> band6 = std::make_shared<Band>(_strip6, 5, 0, 20);
  std::shared_ptr<Band> band7 = std::make_shared<Band>(_strip7, 6, 0, 20);

  _analyzer = std::make_shared<Analyzer>();
  _analyzer->setBand(band1);
  _analyzer->setBand(band2);
  _analyzer->setBand(band3);
  _analyzer->setBand(band4);
  _analyzer->setBand(band5);
  _analyzer->setBand(band6);
  _analyzer->setBand(band7);
  _analyzer->setup();
}

static void simulationTask()
{
  static std::vector<uint8_t> vSimValues(BANDS, 0);
  static uint32_t u32TimerSimulation = millis();

  if (false == isTimeExpired(u32TimerSimulation, 200))
    return;
  for (auto &band : vSimValues)
  {
    band = (uint8_t)(1 + (rand() % 100));
  }
  _analyzer->loop(vSimValues);
}

static void ledTest()
{
  constexpr uint8_t STEPS = 1;
  static std::vector<uint8_t> vSimValues(BANDS, 0);
  static uint32_t u32TimerSimulation = millis();
  static uint8_t u8LastBand = 0;
  static uint8_t u8CurrentBand = 0;

  if (vSimValues[u8LastBand] && u8CurrentBand != u8LastBand)
    vSimValues[u8LastBand] -= STEPS;

  vSimValues[u8CurrentBand] += STEPS;
  if (vSimValues[u8CurrentBand] >= 100)
  {
    u8LastBand = u8CurrentBand;
    u8CurrentBand++;
    if (u8CurrentBand >= BANDS)
      u8CurrentBand = 0;
  }
  _analyzer->loop(vSimValues);
}

static void toggleAll()
{
  std::vector<uint8_t> vSimValues(BANDS, 100);
  _analyzer->loop(vSimValues);
}

void morseCode()
{
  constexpr uint32_t UNIT_MS = 500; // Länge eines "Punkts"
  static const char *digitMorse[10] = {
      "-----", // 0
      ".----", // 1
      "..---", // 2
      "...--", // 3
      "....-", // 4
      ".....", // 5
      "-....", // 6
      "--...", // 7
      "---..", // 8
      "----."  // 9
  };

  // feste 7-stellige Zahl: 5 7 2 3 8 9 4 (jede Position -> ein Band)
  static bool initialized = false;
  static std::string number; // Länge muss BANDS sein
  if (!initialized)
  {
    initialized = true;
    number = "5723894";
    // Wenn BANDS != 7, passe Größe an (truncate oder pad mit '0')
    if (number.size() != BANDS)
    {
      number.resize(BANDS, '0');
    }
    Serial.print("Morse-Number: ");
    Serial.println(number.c_str());
  }

  enum EState : uint8_t
  {
    SYMBOL_ON = 0,
    SYMBOL_GAP = 1,
    DIGIT_GAP = 2
  };
  static std::vector<EState> state(BANDS, DIGIT_GAP);
  static std::vector<size_t> symbolIndex(BANDS, 0);
  static std::vector<uint32_t> timer(BANDS, millis());

  uint32_t now = millis();
  const uint32_t GAP_DURATION = 10 * UNIT_MS; // Dauer der Pause zwischen Wiederholungen
  // Prüfe, ob sich alle Bänder gerade in der Wiederholungs-Pause befinden und diese noch läuft.
  bool allInGap = true;
  bool gapActive = false;
  for (size_t b = 0; b < BANDS; ++b)
  {
    if (state[b] != DIGIT_GAP)
    {
      allInGap = false;
      break;
    }
    if (now - timer[b] < GAP_DURATION)
      gapActive = true;
  }

  std::vector<uint8_t> vSimValues(BANDS, 0);

  // Wenn alle Bänder in der Pause sind und die Pause noch aktiv ist => ledTest() anzeigen
  if (allInGap && gapActive)
  {
    _analyzer->loop(vSimValues);
    return;
  }

  for (size_t b = 0; b < BANDS; ++b)
  {
    if (now - timer[b] < 0) // overflow-safety
      timer[b] = now;

    char digitChar = (b < number.size()) ? number[b] : '0';
    const char *morse = digitMorse[digitChar - '0'];

    if (state[b] == SYMBOL_ON)
    {
      char sym = morse[symbolIndex[b]];
      uint32_t dur = (sym == '.') ? UNIT_MS : (3 * UNIT_MS);
      if (now - timer[b] >= dur)
      {
        // Wechsel in Symbol-Gap
        state[b] = SYMBOL_GAP;
        timer[b] = now;
        vSimValues[b] = 0;
      }
      else
      {
        vSimValues[b] = 100;
      }
    }
    else if (state[b] == SYMBOL_GAP)
    {
      // kurze Pause zwischen Symbolen (1 Einheit)
      if (now - timer[b] >= UNIT_MS)
      {
        symbolIndex[b]++;
        if (morse[symbolIndex[b]] == 0)
        {
          // Ende der Ziffer -> Pause zwischen Wiederholung der Ziffer (GAP_DURATION)
          state[b] = DIGIT_GAP;
          timer[b] = now;
          symbolIndex[b] = 0;
          vSimValues[b] = 0;
        }
        else
        {
          // nächstes Symbol ON
          state[b] = SYMBOL_ON;
          timer[b] = now;
          vSimValues[b] = 100;
        }
      }
      else
      {
        vSimValues[b] = 0;
      }
    }
    else // DIGIT_GAP
    {
      // Pause zwischen Wiederholungen der Ziffer
      if (now - timer[b] >= GAP_DURATION)
      {
        // Starte erneut mit erstem Symbol der Ziffer
        state[b] = SYMBOL_ON;
        timer[b] = now;
        symbolIndex[b] = 0;
        vSimValues[b] = 100;
      }
      else
      {
        vSimValues[b] = 0;
      }
    }
  }

  _analyzer->loop(vSimValues);
}

void loop()
{
  WifiTask();

  if (getHtmlValues().bSimulationStart)
    simulationTask();
  else if (getHtmlValues().bLedTestStart)
    ledTest();
  else if (getHtmlValues().bAllToggle)
    toggleAll();
  else
  {
    analyzerFFT_Task();
    _analyzer->loop(getBandsFromFFT());
  }
}