#include "zephyr_sim.h"
#include "AdcSensor.hpp"
#include "VariableFan.hpp"
#include "UartLogger.hpp"
#include "AdvancedTemperatureController.hpp"
#include <iostream>
#include <iomanip>

int main() {
    printf("=== PID Temperature Controller Simulation ===\n");
    printf("Advanced temperature control with PID algorithm\n");
    printf("Press Ctrl+C to stop\n\n");

    // Hardware abstraction layer
    static AdcDriver adc;
    static UartDriver uart;

    // Hardware interfaces
    AdcSensor sensor(adc);
    VariableFan fan;  // Variable speed fan using PWM simulation
    UartLogger logger(uart);

    // PID Controller configuration
    PIDController::Config pid_config;
    pid_config.kp = 3.0f;           // Proportional gain
    pid_config.ki = 0.1f;           // Integral gain  
    pid_config.kd = 0.5f;           // Derivative gain
    pid_config.setpoint = 25.0f;    // Target temperature: 25°C
    pid_config.output_min = 0.0f;   // Minimum fan speed (0%)
    pid_config.output_max = 100.0f; // Maximum fan speed (100%)

    // Advanced temperature controller with PID
    AdvancedTemperatureController controller(sensor, fan, logger, pid_config);

    printf("Initial PID Configuration:\n");
    printf("  Setpoint: %.1f°C\n", pid_config.setpoint);
    printf("  Kp: %.2f, Ki: %.3f, Kd: %.2f\n", pid_config.kp, pid_config.ki, pid_config.kd);
    printf("  Output range: %.0f%% - %.0f%%\n\n", pid_config.output_min, pid_config.output_max);

    int cycle = 0;
    while (true) {
        printf("[Cycle %2d] ", ++cycle);
        
        // Run temperature regulation
        controller.regulate();
        
        // Get detailed status
        const auto& stats = controller.getStatistics();
        const auto& pid_state = controller.getPIDState();
        
        // Display comprehensive status every 5 cycles
        if (cycle % 5 == 0) {
            printf("\n");
            printf("  System Statistics:\n");
            printf("    Total cycles: %d\n", stats.total_cycles);
            printf("    Average temperature: %.2f°C\n", stats.avg_temp);
            printf("    Temperature range: %.1f°C - %.1f°C\n", stats.min_temp, stats.max_temp);
            
            printf("  PID State:\n");
            printf("    Error: %.2f°C\n", pid_state.error);
            printf("    P-term: %.2f, I-term: %.2f, D-term: %.2f\n", 
                   pid_state.p_term, pid_state.i_term, pid_state.d_term);
            printf("    Output: %.1f%%, Integral: %.2f\n", 
                   pid_state.output, pid_state.integral);
            printf("\n");
        }
        
        // Simulate different scenarios
        if (cycle == 15) {
            printf("\n--- Simulating temperature spike to 35°C ---\n");
            // The simulation framework will handle this
        }
        else if (cycle == 30) {
            printf("\n--- Adjusting setpoint to 28°C ---\n");
            controller.setSetpoint(28.0f);
        }
        else if (cycle == 45) {
            printf("\n--- Testing different PID gains ---\n");
            controller.tunePID(2.5f, 0.15f, 0.8f);
            printf("Updated gains: Kp=2.5, Ki=0.15, Kd=0.8\n");
        }
        
        k_sleep(K_SECONDS(1));
        
        // Stop after 60 cycles for demonstration
        if (cycle >= 60) {
            printf("\n=== Simulation Complete ===\n");
            printf("Final Statistics:\n");
            printf("  Total cycles: %d\n", stats.total_cycles);
            printf("  Average temperature: %.2f°C\n", stats.avg_temp);
            printf("  Temperature range: %.1f°C - %.1f°C\n", stats.min_temp, stats.max_temp);
            break;
        }
    }

    return 0;
}
