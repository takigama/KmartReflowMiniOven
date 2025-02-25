#pragma once

#include "config.h"
#include "Temperature.h"



class AggregateTemperature
{
public:
    // method for inititing our object
    AggregateTemperature();

    // add a sensor
    bool addSensor(int pin, double referenceResistance, double nominalResistance,
                   double nominalTemperatureCelsius, double bValue, int adcResolution, double idealMinTemp, double idealMaxTemp);

    // get aggregate temperature
    double getCelcius();

    // pull a single temp sensor.
    Thermistor *getThermistorDirect(int n);

private:
    // one array of thermistors please
    Thermistor *thermistors[N_TEMP_SENSORS];

    int nSensorsCreated;
    double maxTemps[N_TEMP_SENSORS];
    double minTemps[N_TEMP_SENSORS];
};