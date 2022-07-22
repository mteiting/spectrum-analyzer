#pragma once

#include <Arduino.h>
#include <vector>

void analyzerFFT_Setup();
void analyzerFFT_Task();
std::vector<uint8_t> &getBandsFromFFT();