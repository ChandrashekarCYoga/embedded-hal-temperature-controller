#include <zephyr.h>
#include "AdcSensor.hpp"
#include "GpioFan.hpp"
#include "UartLogger.hpp"
#include "TemperatureController.hpp"

extern "C" void main(void) {
    static AdcDriver adc;
    static GpioDriver gpio;
    static UartDriver uart;

    AdcSensor sensor(adc);
    GpioFan fan(gpio);
    UartLogger logger(uart);

    TemperatureController controller(sensor, fan, logger);

    while (true) {
        controller.regulate();
        k_sleep(K_SECONDS(1));
    }
}
