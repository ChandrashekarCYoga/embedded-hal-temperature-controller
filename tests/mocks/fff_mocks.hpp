#pragma once

#define FFF_INCLUDE_FFP_FUNC
#include "../include/fff.h"
#include <cstdint>
#include <vector>
#include <string>

// Manual declarations (instead of complex macros)
typedef struct { 
    unsigned int call_count; 
    uint16_t return_val; 
    uint16_t return_val_seq[50]; 
    unsigned int return_val_seq_len; 
    unsigned int return_val_seq_idx; 
} adc_read_raw_fake_t;

typedef struct { unsigned int call_count; } gpio_set_high_fake_t;
typedef struct { unsigned int call_count; } gpio_set_low_fake_t;
typedef struct { 
    unsigned int call_count; 
    bool return_val; 
    bool return_val_seq[50]; 
    unsigned int return_val_seq_len; 
    unsigned int return_val_seq_idx; 
} gpio_get_state_fake_t;

typedef struct { 
    unsigned int call_count; 
    const char* arg0_history[50]; 
} uart_write_fake_t;

// Mock function declarations for ADC driver
extern adc_read_raw_fake_t adc_read_raw_fake;
extern uint16_t adc_read_raw_return_val;
extern unsigned int adc_read_raw_call_count;
uint16_t adc_read_raw(void);
void adc_read_raw_reset(void);

// Mock function declarations for GPIO driver  
extern gpio_set_high_fake_t gpio_set_high_fake;
extern unsigned int gpio_set_high_call_count;
void gpio_set_high(void);
void gpio_set_high_reset(void);

extern gpio_set_low_fake_t gpio_set_low_fake;
extern unsigned int gpio_set_low_call_count;
void gpio_set_low(void);
void gpio_set_low_reset(void);

extern gpio_get_state_fake_t gpio_get_state_fake;
extern bool gpio_get_state_return_val;
extern unsigned int gpio_get_state_call_count;
bool gpio_get_state(void);
void gpio_get_state_reset(void);

// Mock function declarations for UART driver
extern uart_write_fake_t uart_write_fake;
extern unsigned int uart_write_call_count;
extern const char* uart_write_arg0_history[50];
void uart_write(const char* arg0);
void uart_write_reset(void);

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
    virtual bool getState() = 0;
    virtual ~GpioDriver() = default;
};

class UartDriver {
public:
    virtual void write(const char* msg) = 0;
    virtual ~UartDriver() = default;
};

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
    
    bool getState() override {
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
    adc_read_raw_reset();
    gpio_set_high_reset();
    gpio_set_low_reset();
    gpio_get_state_reset();
    uart_write_reset();
    
    // Reset mock instances
    extern MockUartDriver mock_uart;
    mock_uart.clear();
}
