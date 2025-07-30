#include "zephyr_sim.h"
#include "AdcSensor.hpp"
#include "GpioFan.hpp"
#include "UartLogger.hpp"
#include "TemperatureController.hpp"

int main() {
    printf("=== Embedded HAL Temperature Controller Simulation ===\n");
    printf("Press Ctrl+C to stop\n\n");

    static AdcDriver adc;
    static GpioDriver gpio;
    static UartDriver uart;

    AdcSensor sensor(adc);
    GpioFan fan(gpio);
    UartLogger logger(uart);

    TemperatureController controller(sensor, fan, logger);

    int cycle = 0;
    while (true) {
        printf("[Cycle %d] ", ++cycle);
        controller.regulate();
        k_sleep(K_SECONDS(1));
        
        // Simulate some variation after 10 cycles
        if (cycle == 10) {
            printf("\n--- Simulating temperature spike ---\n");
        }
    }

    return 0;
}
