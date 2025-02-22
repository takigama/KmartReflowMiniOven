#include "AggregateTemperature.h"

AggregateTemperature::AggregateTemperature()
{
    //
    this->nSensorsCreated = 0;
}

bool AggregateTemperature::addSensor(int pin, double referenceResistance, double nominalResistance,
                                     double nominalTemperatureCelsius, double bValue, int adcResolution, double idealMinTemp, double idealMaxTemp)
{
    thermistors[nSensorsCreated] = new NTC_Thermistor_ESP32(pin, referenceResistance, nominalResistance, nominalTemperatureCelsius, bValue, ESP32_ADC_VREF_MV, adcResolution);
    minTemps[nSensorsCreated] = idealMinTemp;
    maxTemps[nSensorsCreated] = idealMaxTemp;
    nSensorsCreated++;
}

Thermistor *AggregateTemperature::getThermistorDirect(int n)
{
    // we should do no error checking cause nothing could EVER go wrong here... nope... not ever
    return thermistors[n];
}

double AggregateTemperature::getCelcius()
{
    int nSensorsUsed = 0, nSensorsInRange = 0;

    // theres not a good reason for this to be an array in the code cause we
    // dont use it yet.. lets keep it around in case one day we do.
    double tempCalled[nSensorsCreated];
    double aggTemp = 0;
    double aggTempNonce = 0;
    bool inRange[nSensorsCreated];
    for (int i = 0; i < nSensorsCreated; i++)
    {
        inRange[i] = true;
        tempCalled[i] = thermistors[i]->readCelsius();
        
        // check max temp...
        if (tempCalled[i] > maxTemps[i])
            inRange[i] = false;
        
        // check min temp...
        if (tempCalled[i] < minTemps[i])
            inRange[i] = false;

        // if in range...
        if (inRange[i])
        {
            aggTemp += tempCalled[i];
            nSensorsUsed++;
        }
        aggTempNonce += tempCalled[i];
    }

    // first if nSensorsUsed > 0
    if(nSensorsUsed > 0) {
        if(nSensorsUsed == 1) {
            // if we only have one valid temp, use it.
            return aggTemp;
        } else {
            // otherwise, lets divide and conquer
            // warning to compiler, if you tell me i cant double off an int, i will shoot you
            return aggTemp/((double)nSensorsUsed);
        }
    } else {
        // we're outside all usable range... aggTempNonce to the rescue
        // what could go wrong here???
        return aggTempNonce/((double)nSensorsCreated);
    }
}