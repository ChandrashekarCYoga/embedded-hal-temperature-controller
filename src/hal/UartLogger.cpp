// UartLogger.hpp
#include "ILogger.hpp"
#include <cstdio>
class UartDriver { public: void write(const char*); };
class UartLogger : public ILogger {
public:
    UartLogger(UartDriver& uart): uart_(uart) {}
    void log(float val) override {
        char buf[64];
        int n = snprintf(buf, sizeof(buf), "Temp=%.2f\n", val);
        uart_.write(buf);
    }
private:
    UartDriver& uart_;
};
