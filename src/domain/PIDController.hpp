#pragma once

/**
 * @file PIDController.hpp
 * @brief PID (Proportional-Integral-Derivative) Controller Implementation
 * 
 * Provides sophisticated temperature control with smooth regulation
 * instead of simple on/off control.
 */

#include <cstdint>
#include <algorithm>

class PIDController {
public:
    /**
     * @brief Configuration parameters for PID controller
     */
    struct Config {
        float kp = 2.0f;        // Proportional gain
        float ki = 0.1f;        // Integral gain  
        float kd = 0.5f;        // Derivative gain
        float setpoint = 25.0f; // Target temperature (°C)
        float output_min = 0.0f;   // Minimum output (0%)
        float output_max = 100.0f; // Maximum output (100%)
        float integral_max = 50.0f; // Anti-windup limit
    };

    /**
     * @brief PID controller state and output information
     */
    struct State {
        float error = 0.0f;           // Current error (setpoint - input)
        float error_prev = 0.0f;      // Previous error for derivative
        float integral = 0.0f;        // Accumulated integral term
        float derivative = 0.0f;      // Rate of change (derivative term)
        float output = 0.0f;          // Final PID output (0-100%)
        float p_term = 0.0f;          // Proportional component
        float i_term = 0.0f;          // Integral component  
        float d_term = 0.0f;          // Derivative component
        uint32_t update_count = 0;    // Number of updates performed
        bool first_run = true;        // Flag for first execution
    };

private:
    Config config_;
    State state_;

public:
    /**
     * @brief Construct PID controller with default configuration
     */
    PIDController() = default;

    /**
     * @brief Construct PID controller with custom configuration
     * @param config PID configuration parameters
     */
    explicit PIDController(const Config& config) : config_(config) {}

    /**
     * @brief Update PID controller with new temperature reading
     * @param input Current temperature reading (°C)
     * @return Control output (0-100% fan speed)
     */
    float update(float input) {
        // Calculate error (negative = too hot, positive = too cool)
        state_.error = config_.setpoint - input;

        // Proportional term
        state_.p_term = config_.kp * state_.error;

        // Integral term (with anti-windup)
        state_.integral += state_.error;
        // Prevent integral windup
        state_.integral = std::max(-config_.integral_max, 
                                 std::min(config_.integral_max, state_.integral));
        state_.i_term = config_.ki * state_.integral;

        // Derivative term (skip on first run)
        if (!state_.first_run) {
            state_.derivative = state_.error - state_.error_prev;
            state_.d_term = config_.kd * state_.derivative;
        } else {
            state_.derivative = 0.0f;
            state_.d_term = 0.0f;
            state_.first_run = false;
        }

        // Calculate total output (invert for cooling applications)
        // When error is negative (too hot), we want positive output (fan speed)
        state_.output = -(state_.p_term + state_.i_term + state_.d_term);

        // Clamp output to valid range
        state_.output = std::max(config_.output_min, 
                               std::min(config_.output_max, state_.output));

        // Store error for next derivative calculation
        state_.error_prev = state_.error;
        state_.update_count++;

        return state_.output;
    }

    /**
     * @brief Set new target temperature
     * @param setpoint Target temperature in Celsius
     */
    void setSetpoint(float setpoint) {
        config_.setpoint = setpoint;
    }

    /**
     * @brief Get current target temperature
     * @return Current setpoint in Celsius
     */
    float getSetpoint() const {
        return config_.setpoint;
    }

    /**
     * @brief Update PID gains
     * @param kp Proportional gain
     * @param ki Integral gain
     * @param kd Derivative gain
     */
    void setGains(float kp, float ki, float kd) {
        config_.kp = kp;
        config_.ki = ki;
        config_.kd = kd;
    }

    /**
     * @brief Reset PID controller state
     */
    void reset() {
        state_ = State{};
    }

    /**
     * @brief Get current PID state for debugging/monitoring
     * @return Current controller state
     */
    const State& getState() const {
        return state_;
    }

    /**
     * @brief Get current configuration
     * @return Current PID configuration
     */
    const Config& getConfig() const {
        return config_;
    }

    /**
     * @brief Check if controller output indicates cooling needed
     * @return true if significant cooling output (>10%)
     */
    bool isCoolingActive() const {
        return state_.output > 10.0f;
    }

    /**
     * @brief Get human-readable control status
     * @return Status string
     */
    const char* getStatusString() const {
        if (state_.output < 5.0f) return "OFF";
        else if (state_.output < 25.0f) return "LOW";
        else if (state_.output < 50.0f) return "MED";
        else if (state_.output < 75.0f) return "HIGH";
        else return "MAX";
    }
};
