#include "ztest_framework.hpp"
#include "mocks/fff_mocks.hpp"
#include "AdcSensor.hpp"
#include "TemperatureProcessor.hpp"

// Define MockAdcDriver as AdcDriver for testing
#define AdcDriver MockAdcDriver

ZTEST(adc_sensor, test_reads_value_from_driver) {
    reset_all_fakes();
    MockAdcDriver mock_adc;
    AdcSensor sensor(mock_adc);
    
    // Set up fake to return known value
    adc_read_raw_fake.return_val = 1000;
    
    // Read the temperature
    float temperature = sensor.readValue();
    
    // Verify the conversion is applied
    float expected = TemperatureProcessor::toCelsius(1000);
    zassert_float_equal(temperature, expected, "Should convert ADC value to temperature correctly");
    
    // Verify mock was called
    zassert_equal(adc_read_raw_fake.call_count, 1, "Should call ADC read once");
}

ZTEST(adc_sensor, test_handles_sequence_of_values) {
    reset_all_fakes();
    MockAdcDriver mock_adc;
    AdcSensor sensor(mock_adc);
    
    // Set sequence of return values
    uint16_t sequence[] = {800, 850, 900, 950};
    SET_RETURN_SEQ(adc_read_raw, sequence, 4);
    
    // Read multiple values and verify sequence
    for (int i = 0; i < 4; i++) {
        float temperature = sensor.readValue();
        float expected_temp = TemperatureProcessor::toCelsius(sequence[i]);
        zassert_float_equal(temperature, expected_temp, "Should convert each ADC value correctly");
    }
    
    zassert_equal(adc_read_raw_fake.call_count, 4, "Should call ADC read four times");
}

ZTEST(adc_sensor, test_polymorphic_behavior) {
    reset_all_fakes();
    MockAdcDriver mock_adc;
    AdcSensor sensor(mock_adc);
    
    // Test polymorphic behavior
    ISensor* base_sensor = &sensor;
    adc_read_raw_fake.return_val = 2000;
    
    float temperature = base_sensor->readValue();
    float expected = TemperatureProcessor::toCelsius(2000);
    zassert_float_equal(temperature, expected, "Should work through ISensor interface");
    
    zassert_equal(adc_read_raw_fake.call_count, 1, "Should call ADC read once through interface");
}
