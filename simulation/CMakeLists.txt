cmake_minimum_required(VERSION 3.18.0)
project(embedded_hal_simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add debug symbols
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")

# Define simulation build
add_definitions(-DSIMULATION_BUILD=1)

# Include directories
include_directories(
    ../src/hal
    ../src/domain
    ../src/app
    .
)

# Source files
add_executable(hal_simulation
    main_sim.cpp
    zephyr_sim.cpp
)

# PID simulation
add_executable(pid_simulation
    pid_sim.cpp
    zephyr_sim.cpp
)

# Link threading library for std::this_thread
find_package(Threads REQUIRED)
target_link_libraries(hal_simulation Threads::Threads)
target_link_libraries(pid_simulation Threads::Threads)
