// AdcSensor.hpp
#include "ISensor.hpp"
#include "TemperatureProcessor.hpp"
#include <zephyr.h>
class AdcDriver { /* thin wrapper or Zephyr ADC API usage */ 
public:
    uint16_t readRaw();
};

class AdcSensor : public ISensor {
public:
    AdcSensor(AdcDriver& adc) : adc_(adc) {}
    float readValue() override {
        return TemperatureProcessor::toCelsius(adc_.readRaw());
    }
private:
    AdcDriver& adc_;
};
