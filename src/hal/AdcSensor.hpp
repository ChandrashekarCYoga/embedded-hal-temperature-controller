#pragma once
#include "ISensor.hpp"
#include "TemperatureProcessor.hpp"
#include "drivers.hpp"

class AdcSensor : public ISensor {
public:
    AdcSensor(AdcDriver& adc) : adc_(adc) {}
    float readValue() override {
        return TemperatureProcessor::toCelsius(adc_.readRaw());
    }
private:
    AdcDriver& adc_;
};
