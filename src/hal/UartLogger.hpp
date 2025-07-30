#pragma once
#include "ILogger.hpp"
#include "drivers.hpp"
#include <cstdio>

class UartLogger : public ILogger {
public:
    UartLogger(UartDriver& uart): uart_(uart) {}
    void log(float val) override {
        char buf[64];
        snprintf(buf, sizeof(buf), "Temp=%.2f°C\n", val);
        uart_.write(buf);
    }
private:
    UartDriver& uart_;
};
