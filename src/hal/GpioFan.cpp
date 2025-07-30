// GpioFan.hpp
#include "IActuator.hpp"
class GpioDriver { /* write to port */ public: void setHigh(); void setLow(); };
class GpioFan : public IActuator {
public:
    GpioFan(GpioDriver& gpio) : gpio_(gpio) {}
    void activate() override { gpio_.setHigh(); }
    void deactivate() override { gpio_.setLow(); }
private:
    GpioDriver& gpio_;
};
