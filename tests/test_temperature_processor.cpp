#include "ztest_framework.hpp"
#include "TemperatureProcessor.hpp"

ZTEST(temperature_processor, test_to_celsius_basic_values) {
    // Test typical ADC values
    float result = TemperatureProcessor::toCelsius(0);
    zassert_float_equal(result, 0.0f, "ADC value 0 should convert to 0°C");
    
    result = TemperatureProcessor::toCelsius(4095);
    zassert_float_equal(result, 330.0f, "ADC value 4095 should convert to 330°C");
    
    result = TemperatureProcessor::toCelsius(2048);
    zassert_float_equal(result, 165.0f, "ADC value 2048 should convert to 165°C");
}

ZTEST(temperature_processor, test_to_celsius_edge_cases) {
    // Test edge cases - allow for some tolerance
    float result1 = TemperatureProcessor::toCelsius(1);
    zassert_true(result1 > 0.08f && result1 < 0.082f, "ADC value 1 should be around 0.081°C");
    
    float result2 = TemperatureProcessor::toCelsius(4094);
    zassert_true(result2 > 329.9f && result2 < 330.0f, "ADC value 4094 should be around 329.95°C");
}

ZTEST(temperature_processor, test_to_celsius_realistic_readings) {
    // Test realistic temperature sensor readings
    uint16_t adc_25c = 800;
    float temp_25c = TemperatureProcessor::toCelsius(adc_25c);
    zassert_true(temp_25c > 63.5f && temp_25c < 65.5f, "ADC 800 should be around 64.5°C ± 1°C");
    
    uint16_t adc_30c = 850;
    float temp_30c = TemperatureProcessor::toCelsius(adc_30c);
    zassert_true(temp_30c > 67.5f && temp_30c < 69.5f, "ADC 850 should be around 68.5°C ± 1°C");
}
