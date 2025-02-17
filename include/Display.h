#pragma once
#include "config.h"

void updateDisplay(int duty, int temp);
void initDisplay();

extern TFT_eSPI *tft; // Invoke library, pins defined in User_Setup.h

