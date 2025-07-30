#pragma once

/**
 * @file IVariableActuator.hpp
 * @brief Variable-speed actuator interface for PID control
 * 
 * Extends the basic on/off actuator to support variable output
 * for smooth, proportional control.
 */

class IVariableActuator {
public:
    virtual ~IVariableActuator() = default;

    /**
     * @brief Set actuator output as percentage
     * @param percent Output percentage (0.0 = off, 100.0 = full power)
     */
    virtual void setOutput(float percent) = 0;

    /**
     * @brief Get current output percentage
     * @return Current output level (0-100%)
     */
    virtual float getOutput() const = 0;

    /**
     * @brief Check if actuator is currently active
     * @return true if output > 0%
     */
    virtual bool isActive() const = 0;

    // Backward compatibility with existing IActuator interface
    virtual void activate() {
        setOutput(100.0f);
    }

    virtual void deactivate() {
        setOutput(0.0f);
    }
};
