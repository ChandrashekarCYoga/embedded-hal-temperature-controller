cmake_minimum_required(VERSION 3.18.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(embedded_hal_project)

target_sources(app PRIVATE
    src/main.cpp
    src/hal/AdcSensor.cpp
    src/hal/GpioFan.cpp
    src/hal/UartLogger.cpp
    src/domain/TemperatureProcessor.cpp
    src/app/TemperatureController.cpp
)
target_include_directories(app PRIVATE
    src/hal
    src/domain
    src/app
)