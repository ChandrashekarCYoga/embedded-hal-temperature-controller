// TemperatureProcessor.hpp
#pragma once
#include <cstdint>

class TemperatureProcessor {
public:
    static float toCelsius(uint16_t raw) {
        return (raw * 3.3f / 4095) * 100.0f;
    }
};
