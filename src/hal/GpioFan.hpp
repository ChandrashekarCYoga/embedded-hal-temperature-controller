#pragma once
#include "IActuator.hpp"
#include "drivers.hpp"

class GpioFan : public IActuator {
public:
    GpioFan(GpioDriver& gpio) : gpio_(gpio) {}
    void activate() override { gpio_.setHigh(); }
    void deactivate() override { gpio_.setLow(); }
private:
    GpioDriver& gpio_;
};
