// TemperatureController.hpp
#include "ISensor.hpp"
#include "IActuator.hpp"
#include "ILogger.hpp"

class TemperatureController {
public:
    TemperatureController(ISensor& s, IActuator& a, ILogger& logger)
        : sensor_(s), actuator_(a), logger_(logger) {}

    void regulate() {
        float temp = sensor_.readValue();
        logger_.log(temp);
        if (temp > 37.0f) actuator_.activate();
        else actuator_.deactivate();
    }

private:
    ISensor& sensor_;
    IActuator& actuator_;
    ILogger& logger_;
};
