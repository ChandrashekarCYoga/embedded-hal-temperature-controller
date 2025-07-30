#include "ztest_framework.hpp"
#include "mocks/fff_mocks.hpp"
#include "GpioFan.hpp"

// Define MockGpioDriver as GpioDriver for testing
#define GpioDriver MockGpioDriver

ZTEST(gpio_fan, test_activate_turns_fan_on) {
    reset_all_fakes();
    MockGpioDriver mock_gpio;
    GpioFan fan(mock_gpio);
    
    // Activate the fan
    fan.activate();
    
    // Verify GPIO was set high
    zassert_equal(gpio_set_high_fake.call_count, 1, "Should call gpio_set_high once");
    zassert_equal(gpio_set_low_fake.call_count, 0, "Should not call gpio_set_low");
}

ZTEST(gpio_fan, test_deactivate_turns_fan_off) {
    reset_all_fakes();
    MockGpioDriver mock_gpio;
    GpioFan fan(mock_gpio);
    
    // First turn on, then off
    fan.activate();
    fan.deactivate();
    
    // Verify both calls
    zassert_equal(gpio_set_high_fake.call_count, 1, "Should call gpio_set_high once");
    zassert_equal(gpio_set_low_fake.call_count, 1, "Should call gpio_set_low once");
}

ZTEST(gpio_fan, test_multiple_activations) {
    reset_all_fakes();
    MockGpioDriver mock_gpio;
    GpioFan fan(mock_gpio);
    
    // Multiple activate/deactivate cycles
    for (int i = 0; i < 3; i++) {
        fan.activate();
        fan.deactivate();
    }
    
    zassert_equal(gpio_set_high_fake.call_count, 3, "Should call gpio_set_high three times");
    zassert_equal(gpio_set_low_fake.call_count, 3, "Should call gpio_set_low three times");
}

ZTEST(gpio_fan, test_polymorphic_behavior) {
    reset_all_fakes();
    MockGpioDriver mock_gpio;
    GpioFan fan(mock_gpio);
    
    // Test polymorphic behavior
    IActuator* base_actuator = &fan;
    
    base_actuator->activate();
    zassert_equal(gpio_set_high_fake.call_count, 1, "Should work through IActuator interface for activation");
    
    base_actuator->deactivate();
    zassert_equal(gpio_set_low_fake.call_count, 1, "Should work through IActuator interface for deactivation");
}
