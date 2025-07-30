/**
 * @file test_pid_controller.cpp
 * @brief Unit tests for PID Controller
 */

#include "ztest_framework.hpp"
#include "../src/domain/PIDController.hpp"
#include <iostream>

// Test suite for PID Controller

// Test 1: Basic constructor and default values
ZTEST(pid_controller, constructor_default) {
    PIDController pid_default;
    const auto& config = pid_default.getConfig();
    
    zassert_equal(config.kp, 2.0f, "Default Kp should be 2.0");
    zassert_equal(config.ki, 0.1f, "Default Ki should be 0.1");
    zassert_equal(config.kd, 0.5f, "Default Kd should be 0.5");
    zassert_equal(config.setpoint, 25.0f, "Default setpoint should be 25°C");
}

// Test 2: Constructor with custom parameters
ZTEST(pid_controller, constructor_custom) {
    PIDController::Config custom_config;
    custom_config.kp = 1.5f;
    custom_config.ki = 0.2f;
    custom_config.kd = 0.3f;
    custom_config.setpoint = 30.0f;
    
    PIDController pid_custom(custom_config);
    const auto& config = pid_custom.getConfig();
    
    zassert_equal(config.kp, 1.5f, "Custom Kp should be 1.5");
    zassert_equal(config.ki, 0.2f, "Custom Ki should be 0.2");
    zassert_equal(config.kd, 0.3f, "Custom Kd should be 0.3");
    zassert_equal(config.setpoint, 30.0f, "Custom setpoint should be 30°C");
}

// Test 3: Setpoint control
ZTEST(pid_controller, setpoint_control) {
    PIDController pid_test;
    
    pid_test.setSetpoint(35.0f);
    zassert_equal(pid_test.getSetpoint(), 35.0f, "Setpoint should be updated");
    
    pid_test.setSetpoint(20.0f);
    zassert_equal(pid_test.getSetpoint(), 20.0f, "Setpoint should be updated again");
}

// Test 4: Proportional response
ZTEST(pid_controller, proportional_response) {
    PIDController pid_test;
    pid_test.setSetpoint(25.0f);
    
    // Test temperature below setpoint (too cool, needs less cooling/heating)
    float output1 = pid_test.update(20.0f); // 5°C below setpoint
    zassert_true(output1 <= 0.0f, "Output should be low/negative when temp below setpoint (less cooling needed)");
    
    pid_test.reset();
    
    // Test temperature above setpoint (needs cooling, output should be positive/high)
    float output2 = pid_test.update(30.0f); // 5°C above setpoint
    zassert_true(output2 > 0.0f, "Output should be positive when temp above setpoint (cooling needed)");
}

// Test 5: Integral accumulation
ZTEST(pid_controller, integral_accumulation) {
    PIDController::Config config;
    config.kp = 1.0f;
    config.ki = 0.5f; // Significant integral gain
    config.kd = 0.0f;
    config.setpoint = 25.0f;
    
    PIDController pid_test(config);
    
    // Apply constant error over time
    float output1 = pid_test.update(30.0f); // Error = -5
    float output2 = pid_test.update(30.0f); // Error = -5 (accumulated)
    float output3 = pid_test.update(30.0f); // Error = -5 (more accumulated)
    
    // Integral term should make output progressively more positive (cooling demand increases)
    zassert_true(output2 > output1, "Integral should accumulate error (increasing cooling demand)");
    zassert_true(output3 > output2, "Integral should continue accumulating");
}

// Test 6: Derivative response
ZTEST(pid_controller, derivative_response) {
    PIDController::Config config;
    config.kp = 0.0f;
    config.ki = 0.0f;  
    config.kd = 2.0f; // Only derivative term
    config.setpoint = 25.0f;
    
    PIDController pid_test(config);
    
    // First update (no derivative on first run)
    float output1 = pid_test.update(20.0f);
    zassert_equal(output1, 0.0f, "No derivative on first update");
    
    // Second update with changing error
    float output2 = pid_test.update(22.0f); // Error changed from +5 to +3 (error decreasing)
    zassert_true(output2 > 0.0f, "Derivative should respond to error change (decreasing error should give positive derivative response)");
}

// Test 7: Output clamping
ZTEST(pid_controller, output_clamping) {
    PIDController::Config config;
    config.kp = 100.0f; // Very high gain to force clamping
    config.ki = 0.0f;
    config.kd = 0.0f;
    config.setpoint = 25.0f;
    config.output_min = 0.0f;
    config.output_max = 100.0f;
    
    PIDController pid_test(config);
    
    // Test upper limit (very hot temperature needs maximum cooling)
    float output_high = pid_test.update(50.0f); // Much hotter than setpoint
    zassert_equal(output_high, 100.0f, "Output should be clamped to maximum");
    
    pid_test.reset();
    
    // Test lower limit (very cool temperature needs minimum cooling)
    float output_low = pid_test.update(0.0f); // Much cooler than setpoint
    zassert_equal(output_low, 0.0f, "Output should be clamped to minimum");
}

// Test 8: Reset functionality
ZTEST(pid_controller, reset_functionality) {
    PIDController pid_test;
    
    // Run a few updates to build up state
    pid_test.update(30.0f);
    pid_test.update(32.0f);
    pid_test.update(28.0f);
    
    // Verify state has been built up
    const auto& state_before = pid_test.getState();
    zassert_true(state_before.update_count > 0, "Should have update count");
    zassert_true(state_before.integral != 0.0f, "Should have integral accumulation");
    
    // Reset and verify clean state
    pid_test.reset();
    const auto& state_after = pid_test.getState();
    zassert_equal(state_after.update_count, 0, "Update count should be reset");
    zassert_equal(state_after.integral, 0.0f, "Integral should be reset");
    zassert_equal(state_after.error_prev, 0.0f, "Previous error should be reset");
    zassert_true(state_after.first_run, "First run flag should be reset");
}

// Test 9: Cooling detection
ZTEST(pid_controller, cooling_detection) {
    PIDController pid_test;
    pid_test.setSetpoint(25.0f);
    
    // Temperature at setpoint - no cooling needed
    pid_test.update(25.0f);
    zassert_false(pid_test.isCoolingActive(), "No cooling needed at setpoint");
    
    // High temperature - cooling should be active
    pid_test.update(35.0f);
    zassert_true(pid_test.isCoolingActive(), "Cooling should be active for high temp");
}

// Test 10: Status strings
ZTEST(pid_controller, status_strings) {
    PIDController pid_test;
    pid_test.setSetpoint(25.0f);
    
    // Test different output levels
    pid_test.update(25.0f); // Should be near 0
    const char* status_off = pid_test.getStatusString();
    zassert_str_equal(status_off, "OFF", "Status should be OFF for low output");
    
    // Force medium output by adjusting setpoint
    pid_test.setSetpoint(20.0f);
    pid_test.update(25.0f); // Should give medium output
    // Note: Exact status depends on PID gains, but should not be OFF
    const char* status_active = pid_test.getStatusString();
    zassert_str_not_equal(status_active, "OFF", "Status should not be OFF for active control");
}

// Test 11: Gains update
ZTEST(pid_controller, gains_update) {
    PIDController pid_test;
    
    pid_test.setGains(3.0f, 0.2f, 0.8f);
    const auto& config = pid_test.getConfig();
    
    zassert_equal(config.kp, 3.0f, "Kp should be updated");
    zassert_equal(config.ki, 0.2f, "Ki should be updated");
    zassert_equal(config.kd, 0.8f, "Kd should be updated");
}
