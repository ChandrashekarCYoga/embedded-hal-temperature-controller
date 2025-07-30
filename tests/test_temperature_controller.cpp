#include "ztest_framework.hpp"
#include "mocks/fff_mocks.hpp"
#include "TemperatureController.hpp"
#include "AdcSensor.hpp"
#include "GpioFan.hpp"
#include "UartLogger.hpp"

// Define mock drivers for testing
#define AdcDriver MockAdcDriver
#define GpioDriver MockGpioDriver
#define UartDriver MockUartDriver

// Helper function to set ADC value for specific temperature
uint16_t temperatureToAdc(float celsius) {
    // Reverse calculation: temp = (raw * 3.3 / 4095) * 100
    // raw = (temp * 4095) / (3.3 * 100)
    return static_cast<uint16_t>((celsius * 4095.0f) / 330.0f);
}

ZTEST(temperature_controller, test_normal_temperature_no_fan) {
    reset_all_fakes();
    
    MockAdcDriver mock_adc;
    MockGpioDriver mock_gpio;
    MockUartDriver mock_uart;
    
    AdcSensor sensor(mock_adc);
    GpioFan fan(mock_gpio);
    UartLogger logger(mock_uart);
    TemperatureController controller(sensor, fan, logger);
    
    // Set temperature to 25°C (below threshold)
    adc_read_raw_fake.return_val = temperatureToAdc(25.0f);
    
    controller.regulate();
    
    // Fan should not be activated
    zassert_equal(gpio_set_high_fake.call_count, 0, "Fan should not be activated for normal temperature");
    // But we might call set_low to ensure it's off
    // Logger should be called
    zassert_equal(uart_write_fake.call_count, 1, "Should log temperature reading");
}

ZTEST(temperature_controller, test_high_temperature_activates_fan) {
    reset_all_fakes();
    
    MockAdcDriver mock_adc;
    MockGpioDriver mock_gpio;
    MockUartDriver mock_uart;
    
    AdcSensor sensor(mock_adc);
    GpioFan fan(mock_gpio);
    UartLogger logger(mock_uart);
    TemperatureController controller(sensor, fan, logger);
    
    // Set temperature to 40°C (above threshold)
    adc_read_raw_fake.return_val = temperatureToAdc(40.0f);
    
    controller.regulate();
    
    // Fan should be activated
    zassert_equal(gpio_set_high_fake.call_count, 1, "Fan should be activated for high temperature");
    // Logger should be called
    zassert_equal(uart_write_fake.call_count, 1, "Should log temperature reading");
}

ZTEST(temperature_controller, test_threshold_boundary) {
    reset_all_fakes();
    
    MockAdcDriver mock_adc;
    MockGpioDriver mock_gpio;
    MockUartDriver mock_uart;
    
    AdcSensor sensor(mock_adc);
    GpioFan fan(mock_gpio);
    UartLogger logger(mock_uart);
    TemperatureController controller(sensor, fan, logger);
    
    // Set temperature to exactly 37°C (at threshold)
    adc_read_raw_fake.return_val = temperatureToAdc(37.0f);
    
    controller.regulate();
    
    // Fan should not be activated (threshold is >37°C, not >=37°C)
    zassert_equal(gpio_set_high_fake.call_count, 0, "Fan should not be activated at exactly 37°C");
    // Logger should be called
    zassert_equal(uart_write_fake.call_count, 1, "Should log temperature reading");
}

ZTEST(temperature_controller, test_continuous_operation) {
    reset_all_fakes();
    
    MockAdcDriver mock_adc;
    MockGpioDriver mock_gpio;
    MockUartDriver mock_uart;
    
    AdcSensor sensor(mock_adc);
    GpioFan fan(mock_gpio);
    UartLogger logger(mock_uart);
    TemperatureController controller(sensor, fan, logger);
    
    // Test sequence: low -> high -> low temperature
    uint16_t values[] = {
        temperatureToAdc(25.0f),  // Low temp
        temperatureToAdc(45.0f),  // High temp
        temperatureToAdc(30.0f)   // Low temp again
    };
    SET_RETURN_SEQ(adc_read_raw, values, 3);
    
    // First call: low temperature
    controller.regulate();
    // Second call: high temperature  
    controller.regulate();
    // Third call: low temperature
    controller.regulate();
    
    // Should have called high once and low twice (deactivate calls)
    zassert_equal(gpio_set_high_fake.call_count, 1, "Should activate fan once for high temperature");
    zassert_equal(gpio_set_low_fake.call_count, 2, "Should deactivate fan for low temperatures");
    zassert_equal(uart_write_fake.call_count, 3, "Should log all three readings");
}
