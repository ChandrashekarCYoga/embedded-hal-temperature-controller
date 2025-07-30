#pragma once

/**
 * @file VariableFan.hpp
 * @brief Variable-speed fan implementation using PWM control
 */

#include "IVariableActuator.hpp"
#include <algorithm>

class VariableFan : public IVariableActuator {
private:
    float current_output_ = 0.0f;

public:
    void setOutput(float percent) override {
        // Clamp to valid range
        current_output_ = std::max(0.0f, std::min(100.0f, percent));
        
        // In a real implementation, this would set PWM duty cycle
        // For simulation, we'll just store the value
        
        // Example pseudo-code for real hardware:
        // pwm_set_duty_cycle(PWM_CHANNEL_FAN, current_output_);
    }

    float getOutput() const override {
        return current_output_;
    }

    bool isActive() const override {
        return current_output_ > 0.0f;
    }

    // Additional methods for fan-specific functionality
    
    /**
     * @brief Get fan speed level description
     * @return Human-readable speed description
     */
    const char* getSpeedDescription() const {
        if (current_output_ < 5.0f) return "Stopped";
        else if (current_output_ < 25.0f) return "Low Speed";
        else if (current_output_ < 50.0f) return "Medium Speed";
        else if (current_output_ < 75.0f) return "High Speed";
        else return "Maximum Speed";
    }

    /**
     * @brief Get estimated airflow (for simulation/monitoring)
     * @return Relative airflow (0-100 arbitrary units)
     */
    float getAirflow() const {
        // Non-linear relationship: airflow doesn't scale linearly with PWM
        // This simulates real fan behavior
        if (current_output_ < 5.0f) return 0.0f;
        
        // Minimum start-up threshold + non-linear scaling
        float effective_output = std::max(0.0f, current_output_ - 5.0f) / 95.0f;
        return effective_output * effective_output * 100.0f; // Quadratic relationship
    }
};
