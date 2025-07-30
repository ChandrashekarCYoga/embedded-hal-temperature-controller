#pragma once

// Platform abstraction for hardware drivers
// Each platform provides its own implementation

#ifdef TESTING_BUILD
    // For testing, the driver classes are defined in the test mocks
    // Don't define anything here to avoid conflicts
#elif SIMULATION_BUILD
    // Simulation drivers (uses console/mock hardware)
    #include <cstdint>
    #include <cstdio>
    
    class AdcDriver {
    private:
        static uint16_t counter;
    public:
        uint16_t readRaw() {
            // Simulate temperature sensor readings (25-35°C range)
            counter = (counter + 50) % 1000;
            return 800 + counter; // Simulates varying temperature
        }
    };
    
    // Static definition for simulation
    uint16_t AdcDriver::counter = 0;
    
    class GpioDriver {
    private:
        bool pin_state = false;
    public:
        void setHigh() {
            pin_state = true;
            printf("[GPIO] Fan ON\n");
        }
        
        void setLow() {
            pin_state = false;
            printf("[GPIO] Fan OFF\n");
        }
        
        bool getState() const { return pin_state; }
    };
    
    class UartDriver {
    public:
        void write(const char* msg) {
            printf("[UART] %s", msg);
            fflush(stdout);
        }
    };

#else
    // Real hardware drivers (Zephyr)
    #include <zephyr.h>
    
    class AdcDriver {
        // Real Zephyr ADC implementation
    public:
        uint16_t readRaw();
    };
    
    class GpioDriver {
        // Real Zephyr GPIO implementation
    public:
        void setHigh();
        void setLow();
    };
    
    class UartDriver {
        // Real Zephyr UART implementation
    public:
        void write(const char* msg);
    };

#endif
