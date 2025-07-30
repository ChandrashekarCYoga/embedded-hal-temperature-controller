#include "fff_mocks.hpp"

// Define FFF globals here (only once)
DEFINE_FFF_GLOBALS

// Define the fake functions
DEFINE_FAKE_VALUE_FUNC(uint16_t, adc_read_raw);
DEFINE_FAKE_VOID_FUNC(gpio_set_high);
DEFINE_FAKE_VOID_FUNC(gpio_set_low);
DEFINE_FAKE_VALUE_FUNC(bool, gpio_get_state);
DEFINE_FAKE_VOID_FUNC(uart_write, const char*);

// Global mock instances
MockAdcDriver mock_adc;
MockGpioDriver mock_gpio;
MockUartDriver mock_uart;

// Reset all mocks function
extern "C" void reset_all_mocks() {
    reset_all_fakes();
}
