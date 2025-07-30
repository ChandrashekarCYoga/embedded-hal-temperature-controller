#include "fff_mocks.hpp"

// Define FFF globals here (only once)
fff_globals_t fff = {0};

// Manual fake function implementations (instead of complex macros)

// ADC fake function
adc_read_raw_fake_t adc_read_raw_fake = {0};
uint16_t adc_read_raw_return_val = 0;
unsigned int adc_read_raw_call_count = 0;

uint16_t adc_read_raw(void) {
    adc_read_raw_call_count++;
    adc_read_raw_fake.call_count++;
    if(adc_read_raw_fake.return_val_seq_len > 0) {
        if(adc_read_raw_fake.return_val_seq_idx < adc_read_raw_fake.return_val_seq_len) {
            return adc_read_raw_fake.return_val_seq[adc_read_raw_fake.return_val_seq_idx++];
        } else {
            return adc_read_raw_fake.return_val_seq[adc_read_raw_fake.return_val_seq_len - 1];
        }
    }
    return adc_read_raw_fake.return_val;
}

void adc_read_raw_reset(void) {
    adc_read_raw_call_count = 0;
    adc_read_raw_fake.call_count = 0;
    adc_read_raw_fake.return_val = 0;
    adc_read_raw_fake.return_val_seq_len = 0;
    adc_read_raw_fake.return_val_seq_idx = 0;
    adc_read_raw_return_val = 0;
}

// GPIO fake functions
gpio_set_high_fake_t gpio_set_high_fake = {0};
unsigned int gpio_set_high_call_count = 0;
void gpio_set_high(void) { gpio_set_high_call_count++; gpio_set_high_fake.call_count++; }
void gpio_set_high_reset(void) { gpio_set_high_call_count = 0; gpio_set_high_fake.call_count = 0; }

gpio_set_low_fake_t gpio_set_low_fake = {0};
unsigned int gpio_set_low_call_count = 0;
void gpio_set_low(void) { gpio_set_low_call_count++; gpio_set_low_fake.call_count++; }
void gpio_set_low_reset(void) { gpio_set_low_call_count = 0; gpio_set_low_fake.call_count = 0; }

gpio_get_state_fake_t gpio_get_state_fake = {0};
bool gpio_get_state_return_val = false;
unsigned int gpio_get_state_call_count = 0;
bool gpio_get_state(void) { 
    gpio_get_state_call_count++; 
    gpio_get_state_fake.call_count++; 
    return gpio_get_state_fake.return_val; 
}
void gpio_get_state_reset(void) { 
    gpio_get_state_call_count = 0; 
    gpio_get_state_fake.call_count = 0; 
    gpio_get_state_fake.return_val = false; 
    gpio_get_state_return_val = false; 
}

// UART fake function
uart_write_fake_t uart_write_fake = {0};
unsigned int uart_write_call_count = 0;
const char* uart_write_arg0_history[50];

void uart_write(const char* arg0) {
    if(uart_write_call_count < 50) {
        uart_write_arg0_history[uart_write_call_count] = arg0;
        uart_write_fake.arg0_history[uart_write_call_count] = arg0;
    }
    uart_write_call_count++;
    uart_write_fake.call_count++;
}

void uart_write_reset(void) {
    uart_write_call_count = 0;
    uart_write_fake.call_count = 0;
}

// Global mock instances
MockAdcDriver mock_adc;
MockGpioDriver mock_gpio;
MockUartDriver mock_uart;

// Reset all mocks function
extern "C" void reset_all_mocks() {
    reset_all_fakes();
}
