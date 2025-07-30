cmake_minimum_required(VERSION 3.18.0)
project(embedded_hal_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add debug symbols for better debugging
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

# Define testing build (similar to simulation)
add_definitions(-DSIMULATION_BUILD=1 -DTESTING_BUILD=1)

# Include Ztest and FFF frameworks from Zephyr
set(ZTEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../zephyr/subsys/testsuite/ztest/include")
set(FFF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../zephyr/subsys/testsuite/include")

# Include directories
include_directories(
    ../src/hal
    ../src/domain
    ../src/app
    ${ZTEST_PATH}
    ${FFF_PATH}
    .
)

# Test sources
set(TEST_SOURCES
    test_main.cpp
    test_temperature_processor.cpp
    test_adc_sensor.cpp
    test_gpio_fan.cpp
    test_uart_logger.cpp
    test_temperature_controller.cpp
    mocks/fff_mocks.cpp
)

# Create test executable
add_executable(unit_tests ${TEST_SOURCES})

# Add a custom target to run tests
add_custom_target(run_tests
    COMMAND unit_tests
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
