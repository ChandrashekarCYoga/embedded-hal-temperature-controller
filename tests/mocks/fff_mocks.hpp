#pragma once

#define FFF_INCLUDE_FFP_FUNC
#include <zephyr/fff.h>
#include <cstdint>
#include <vector>
#include <string>

// Base driver classes (same interface as in src/hal/drivers.hpp)
class AdcDriver {
public:
    virtual uint16_t readRaw() = 0;
    virtual ~AdcDriver() = default;
};

class GpioDriver {
public:
    virtual void setHigh() = 0;
    virtual void setLow() = 0;
    virtual bool getState() const = 0;
    virtual ~GpioDriver() = default;
};

class UartDriver {
public:
    virtual void write(const char* msg) = 0;
    virtual ~UartDriver() = default;
};

// Mock function declarations for ADC driver
DECLARE_FAKE_VALUE_FUNC(uint16_t, adc_read_raw);

// Mock function declarations for GPIO driver  
DECLARE_FAKE_VOID_FUNC(gpio_set_high);
DECLARE_FAKE_VOID_FUNC(gpio_set_low);
DECLARE_FAKE_VALUE_FUNC(bool, gpio_get_state);

// Mock function declarations for UART driver
DECLARE_FAKE_VOID_FUNC(uart_write, const char*);

// Mock driver classes that inherit from base interfaces
class MockAdcDriver : public AdcDriver {
public:
    uint16_t readRaw() override {
        return adc_read_raw();
    }
};

class MockGpioDriver : public GpioDriver {
public:
    void setHigh() override {
        gpio_set_high();
    }
    
    void setLow() override {
        gpio_set_low();
    }
    
    bool getState() const override {
        return gpio_get_state();
    }
};

class MockUartDriver : public UartDriver {
private:
    std::vector<std::string> captured_messages;
    
public:
    void write(const char* msg) override {
        captured_messages.push_back(std::string(msg));
        uart_write(msg);
    }
    
    const std::vector<std::string>& getMessages() const {
        return captured_messages;
    }
    
    std::string getLastMessage() const {
        return captured_messages.empty() ? "" : captured_messages.back();
    }
    
    void clear() {
        captured_messages.clear();
    }
};

// Reset all fakes
inline void reset_all_fakes() {
    RESET_FAKE(adc_read_raw);
    RESET_FAKE(gpio_set_high);
    RESET_FAKE(gpio_set_low);
    RESET_FAKE(gpio_get_state);
    RESET_FAKE(uart_write);
    FFF_RESET_HISTORY();
    
    // Reset mock instances
    extern MockUartDriver mock_uart;
    mock_uart.clear();
}
