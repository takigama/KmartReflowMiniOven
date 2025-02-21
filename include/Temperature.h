#pragma once

#include "config.h"
#include <Thermistor.h>
#include <NTC_Thermistor.h> 

extern Thermistor* thermistor;

void initTemp(int pin, double referenceResistance, double nominalResistance, double nominalTemperatureCelsius, double bValue, int adcResolution = 1023);

double getTemp();
