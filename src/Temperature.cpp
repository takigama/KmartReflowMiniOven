#include "Temperature.h"

Thermistor* thermistor;


void initTemp(int pin, double referenceResistance, double nominalResistance, double nominalTemperatureCelsius, double bValue, int adcResolution)
{
    // thermistor = new NTC_Thermistor(pin, referenceResistance, nominalResistance, nominalTemperatureCelsius, bValue, adcResolution);
    thermistor = new NTC_Thermistor_ESP32(pin, referenceResistance, nominalResistance, nominalTemperatureCelsius, bValue, ESP32_ADC_VREF_MV, adcResolution);

}

double getTemp()
{
  return thermistor->readCelsius();
}