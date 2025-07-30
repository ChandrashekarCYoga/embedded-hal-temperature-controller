#pragma once

/**
 * @file AdvancedTemperatureController.hpp
 * @brief Advanced temperature controller with PID control
 * 
 * Replaces simple on/off control with sophisticated PID regulation
 * for smooth, precise temperature control.
 */

#include "ISensor.hpp"
#include "IVariableActuator.hpp"
#include "ILogger.hpp"
#include "PIDController.hpp"

class AdvancedTemperatureController {
private:
    ISensor& sensor_;
    IVariableActuator& actuator_;
    ILogger& logger_;
    PIDController pid_;
    
    // Statistics and monitoring
    struct Statistics {
        float min_temp = 999.0f;
        float max_temp = -999.0f;
        float avg_temp = 0.0f;
        uint32_t sample_count = 0;
        float temp_sum = 0.0f;
        uint32_t cycles_active = 0;
        uint32_t total_cycles = 0;
    } stats_;

public:
    /**
     * @brief Construct advanced temperature controller
     * @param sensor Temperature sensor reference
     * @param actuator Variable-speed actuator reference  
     * @param logger Logging interface reference
     * @param pid_config Optional PID configuration
     */
    AdvancedTemperatureController(ISensor& sensor, 
                                IVariableActuator& actuator,
                                ILogger& logger,
                                const PIDController::Config& pid_config = PIDController::Config{})
        : sensor_(sensor), actuator_(actuator), logger_(logger), pid_(pid_config) {}

    /**
     * @brief Main regulation cycle - call this periodically
     */
    void regulate() {
        // Read current temperature
        float current_temp = sensor_.readValue();
        
        // Update statistics
        updateStatistics(current_temp);
        
        // Run PID controller
        float control_output = pid_.update(current_temp);
        
        // Apply control output to actuator
        actuator_.setOutput(control_output);
        
        // Enhanced logging with PID data
        logDetailedStatus(current_temp, control_output);
        
        stats_.total_cycles++;
        if (actuator_.isActive()) {
            stats_.cycles_active++;
        }
    }

    /**
     * @brief Set new target temperature
     * @param setpoint Target temperature in Celsius
     */
    void setSetpoint(float setpoint) {
        pid_.setSetpoint(setpoint);
    }

    /**
     * @brief Get current target temperature
     * @return Current setpoint in Celsius
     */
    float getSetpoint() const {
        return pid_.getSetpoint();
    }

    /**
     * @brief Update PID tuning parameters
     * @param kp Proportional gain
     * @param ki Integral gain
     * @param kd Derivative gain
     */
    void tunePID(float kp, float ki, float kd) {
        pid_.setGains(kp, ki, kd);
    }

    /**
     * @brief Reset controller state and statistics
     */
    void reset() {
        pid_.reset();
        stats_ = Statistics{};
    }

    /**
     * @brief Get PID controller state for monitoring
     * @return Current PID state
     */
    const PIDController::State& getPIDState() const {
        return pid_.getState();
    }

    /**
     * @brief Get controller statistics
     * @return Current statistics
     */
    const Statistics& getStatistics() const {
        return stats_;
    }

    /**
     * @brief Get control efficiency (percentage of time active)
     * @return Efficiency percentage (0-100%)
     */
    float getControlEfficiency() const {
        if (stats_.total_cycles == 0) return 0.0f;
        return (float)stats_.cycles_active / stats_.total_cycles * 100.0f;
    }

private:
    /**
     * @brief Update running statistics
     * @param temp Current temperature reading
     */
    void updateStatistics(float temp) {
        stats_.min_temp = std::min(stats_.min_temp, temp);
        stats_.max_temp = std::max(stats_.max_temp, temp);
        stats_.temp_sum += temp;
        stats_.sample_count++;
        stats_.avg_temp = stats_.temp_sum / stats_.sample_count;
    }

    /**
     * @brief Enhanced logging with PID information
     * @param temp Current temperature
     * @param output Control output percentage
     */
    void logDetailedStatus(float temp, float output) {
        const auto& pid_state = pid_.getState();
        
        // Use printf for detailed logging since ILogger only supports float values
        printf("Temp=%.1f°C -> Output=%.1f%% (P=%.1f I=%.1f D=%.1f) [%s]\n",
                temp, output,
                pid_state.p_term, pid_state.i_term, pid_state.d_term,
                pid_.getStatusString());
        
        // Also log the temperature value to the logger interface
        logger_.log(temp);
    }
};
