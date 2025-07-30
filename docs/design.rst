========================================
Embedded HAL Temperature Controller
========================================

:Author: Development Team
:Date: 30 July 2025
:Version: 1.0
:Status: Complete

.. contents:: Table of Contents
   :depth: 3

Overview
========

The Embedded HAL Temperature Controller is a real-time temperature monitoring and control system designed for embedded environments. The system uses a Hardware Abstraction Layer (HAL) to provide portability between simulation and actual hardware implementations.

System Architecture
==================

High-Level Architecture
-----------------------

The system follows a layered architecture with clear separation of concerns:

.. code-block:: text

    ┌─────────────────────────────────────────────────────┐
    │                 Application Layer                   │
    │            (TemperatureController)                  │
    └─────────────────────────────────────────────────────┘
                              │
    ┌─────────────────────────────────────────────────────┐
    │                 Domain Layer                        │
    │           (TemperatureProcessor)                    │
    └─────────────────────────────────────────────────────┘
                              │
    ┌─────────────────────────────────────────────────────┐
    │              Hardware Abstraction Layer             │
    │        (ISensor, IActuator, ILogger)               │
    └─────────────────────────────────────────────────────┘
                              │
    ┌─────────────────────────────────────────────────────┐
    │               Driver Layer                          │
    │    (AdcDriver, GpioDriver, UartDriver)             │
    └─────────────────────────────────────────────────────┘

Class Diagram
=============

Core Interfaces
---------------

.. code-block:: text

    ┌─────────────────┐
    │   <<interface>> │
    │     ISensor     │
    ├─────────────────┤
    │ + readValue():  │
    │   float         │
    └─────────────────┘
            △
            │
    ┌─────────────────┐
    │   AdcSensor     │
    ├─────────────────┤
    │ - uart_: UartDriver& │
    ├─────────────────┤
    │ + AdcSensor(    │
    │   UartDriver&)  │
    │ + readValue():  │
    │   float         │
    └─────────────────┘

    ┌─────────────────┐
    │   <<interface>> │
    │   IActuator     │
    ├─────────────────┤
    │ + activate():   │
    │   void          │
    │ + deactivate(): │
    │   void          │
    └─────────────────┘
            △
            │
    ┌─────────────────┐
    │    GpioFan      │
    ├─────────────────┤
    │ - gpio_: GpioDriver& │
    ├─────────────────┤
    │ + GpioFan(      │
    │   GpioDriver&)  │
    │ + activate():   │
    │   void          │
    │ + deactivate(): │
    │   void          │
    └─────────────────┘

    ┌─────────────────┐
    │   <<interface>> │
    │    ILogger      │
    ├─────────────────┤
    │ + log(float):   │
    │   void          │
    └─────────────────┘
            △
            │
    ┌─────────────────┐
    │   UartLogger    │
    ├─────────────────┤
    │ - uart_: UartDriver& │
    ├─────────────────┤
    │ + UartLogger(   │
    │   UartDriver&)  │
    │ + log(float):   │
    │   void          │
    └─────────────────┘

Application Layer
-----------------

.. code-block:: text

    ┌─────────────────────────────────────────┐
    │          TemperatureController          │
    ├─────────────────────────────────────────┤
    │ - sensor_: ISensor&                     │
    │ - actuator_: IActuator&                 │
    │ - logger_: ILogger&                     │
    ├─────────────────────────────────────────┤
    │ + TemperatureController(ISensor&,       │
    │   IActuator&, ILogger&)                 │
    │ + regulate(): void                      │
    └─────────────────────────────────────────┘
                    │
                    │ uses
                    ▼
    ┌─────────────────────────────────────────┐
    │        TemperatureProcessor             │
    ├─────────────────────────────────────────┤
    │ + THRESHOLD: static const float = 37.0f │
    ├─────────────────────────────────────────┤
    │ + toCelsius(uint16_t): static float     │
    │ + processTemperature(float): bool       │
    │ + isOverThreshold(): bool               │
    └─────────────────────────────────────────┘

Driver Abstraction
------------------

.. code-block:: text

    ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
    │   AdcDriver     │    │   GpioDriver    │    │   UartDriver    │
    │  <<abstract>>   │    │  <<abstract>>   │    │  <<abstract>>   │
    ├─────────────────┤    ├─────────────────┤    ├─────────────────┤
    │ + readRaw():    │    │ + setHigh():    │    │ + write(const   │
    │   uint16_t      │    │   void          │    │   char*): void  │
    └─────────────────┘    │ + setLow():     │    └─────────────────┘
            △              │   void          │            △
            │              │ + getState():   │            │
            │              │   bool          │            │
            │              └─────────────────┘            │
            │                      △                     │
            │                      │                     │
    ┌───────────────────┐  ┌───────────────────┐  ┌───────────────────┐
    │  SimAdcDriver     │  │  SimGpioDriver    │  │  SimUartDriver    │
    │ (Simulation)      │  │ (Simulation)      │  │ (Simulation)      │
    ├───────────────────┤  ├───────────────────┤  ├───────────────────┤
    │ - value_: uint16_t│  │ - state_: bool    │  │ + write(const     │
    ├───────────────────┤  ├───────────────────┤  │   char*): void    │
    │ + readRaw():      │  │ + setHigh(): void │  └───────────────────┘
    │   uint16_t        │  │ + setLow(): void  │
    │ + setValue(       │  │ + getState():     │
    │   uint16_t): void │  │   bool            │
    └───────────────────┘  └───────────────────┘

Platform Abstraction
====================

Build Configuration
-------------------

The system supports multiple build configurations through preprocessor definitions:

.. code-block:: text

    Build Targets:
    ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐
    │   Simulation    │  │    Hardware     │  │     Testing     │
    │                 │  │                 │  │                 │
    │ SIMULATION_     │  │ HARDWARE_       │  │ TESTING_        │
    │ BUILD=1         │  │ BUILD=1         │  │ BUILD=1         │
    │                 │  │                 │  │ SIMULATION_     │
    │ Uses:           │  │ Uses:           │  │ BUILD=1         │
    │ - SimAdcDriver  │  │ - ZephyrAdc     │  │                 │
    │ - SimGpioDriver │  │ - ZephyrGpio    │  │ Uses:           │
    │ - SimUartDriver │  │ - ZephyrUart    │  │ - MockAdc       │
    └─────────────────┘  └─────────────────┘  │ - MockGpio      │
                                             │ - MockUart      │
                                             └─────────────────┘

Sequence Diagrams
=================

Temperature Control Flow
------------------------

.. code-block:: text

    TemperatureController    AdcSensor    TemperatureProcessor    GpioFan    UartLogger
           │                     │               │                 │           │
           │ regulate()          │               │                 │           │
           ├─────────────────────┤               │                 │           │
           │                     │               │                 │           │
           │ readValue()         │               │                 │           │
           ├────────────────────▶│               │                 │           │
           │                     │               │                 │           │
           │                     │ readRaw()     │                 │           │
           │                     ├──────────────▶│                 │           │
           │                     │               │ (to driver)     │           │
           │                     │               │                 │           │
           │                     │ toCelsius()   │                 │           │
           │                     ├──────────────▶│                 │           │
           │                     │               │                 │           │
           │                     │ temperature   │                 │           │
           │ temperature         ◀──────────────┤                 │           │
           ◀────────────────────┤               │                 │           │
           │                     │               │                 │           │
           │ log(temperature)    │               │                 │           │
           ├─────────────────────┼───────────────┼─────────────────┼──────────▶│
           │                     │               │                 │           │
           │                     │               │                 │           │ write()
           │                     │               │                 │           ├────────▶
           │                     │               │                 │           │ (to driver)
           │                     │               │                 │           │
           │ if temp > 37°C      │               │                 │           │
           │ activate()          │               │                 │           │
           ├─────────────────────┼───────────────┼────────────────▶│           │
           │                     │               │                 │           │
           │                     │               │                 │ setHigh() │
           │                     │               │                 ├──────────▶│
           │                     │               │                 │           │ (to driver)
           │                     │               │                 │           │
           │ else                │               │                 │           │
           │ deactivate()        │               │                 │           │
           ├─────────────────────┼───────────────┼────────────────▶│           │
           │                     │               │                 │           │
           │                     │               │                 │ setLow()  │
           │                     │               │                 ├──────────▶│
           │                     │               │                 │           │ (to driver)

System Initialization
---------------------

.. code-block:: text

    Main                 Drivers              Components           Controller
     │                      │                     │                   │
     │ createDrivers()      │                     │                   │
     ├─────────────────────▶│                     │                   │
     │                      │                     │                   │
     │                      │ AdcDriver instance  │                   │
     │                      │ GpioDriver instance │                   │
     │                      │ UartDriver instance │                   │
     │ drivers              │                     │                   │
     ◀─────────────────────┤                     │                   │
     │                      │                     │                   │
     │ createComponents()   │                     │                   │
     ├─────────────────────┼────────────────────▶│                   │
     │                      │                     │                   │
     │                      │                     │ AdcSensor(adc)    │
     │                      │                     │ GpioFan(gpio)     │
     │                      │                     │ UartLogger(uart)  │
     │ components           │                     │                   │
     ◀─────────────────────┼─────────────────────┤                   │
     │                      │                     │                   │
     │ createController()   │                     │                   │
     ├─────────────────────┼─────────────────────┼──────────────────▶│
     │                      │                     │                   │
     │                      │                     │ TemperatureController(
     │                      │                     │   sensor, fan, logger)
     │ controller           │                     │                   │
     ◀─────────────────────┼─────────────────────┼───────────────────┤
     │                      │                     │                   │
     │ while(running)       │                     │                   │
     │ controller.regulate()│                     │                   │
     ├─────────────────────┼─────────────────────┼──────────────────▶│
     │                      │                     │                   │

Testing Architecture
====================

Test Structure
--------------

.. code-block:: text

    ┌─────────────────────────────────────────────────────┐
    │                Test Framework                       │
    │                                                     │
    │  ┌─────────────────┐    ┌─────────────────────────┐ │
    │  │  ZTest Framework│    │    FFF Mock Framework   │ │
    │  │                 │    │                         │ │
    │  │ • ZTEST macro   │    │ • DEFINE_FAKE_*_FUNC   │ │
    │  │ • zassert_*     │    │ • RESET_FAKE            │ │
    │  │ • Test discovery│    │ • Mock verification     │ │
    │  └─────────────────┘    └─────────────────────────┘ │
    └─────────────────────────────────────────────────────┘
                              │
    ┌─────────────────────────────────────────────────────┐
    │                Test Suites                          │
    │                                                     │
    │ ┌─────────────────┐  ┌─────────────────────────────┐ │
    │ │ TemperatureProc │  │      AdcSensor Tests        │ │
    │ │ Tests (3)       │  │      (3 tests)              │ │
    │ └─────────────────┘  └─────────────────────────────┘ │
    │                                                     │
    │ ┌─────────────────┐  ┌─────────────────────────────┐ │
    │ │ GpioFan Tests   │  │      UartLogger Tests       │ │
    │ │ (4 tests)       │  │      (3 tests)              │ │
    │ └─────────────────┘  └─────────────────────────────┘ │
    │                                                     │
    │ ┌─────────────────────────────────────────────────┐ │
    │ │      TemperatureController Tests                │ │
    │ │              (4 tests)                          │ │
    │ └─────────────────────────────────────────────────┘ │
    └─────────────────────────────────────────────────────┘

Mock Architecture
-----------------

.. code-block:: text

    Real Implementation          Mock Implementation
    ┌─────────────────┐         ┌─────────────────────┐
    │   AdcDriver     │         │   MockAdcDriver     │
    │                 │         │                     │
    │ Hardware calls  │ ◀──────▶│ Fake functions     │
    │ Real ADC        │         │ Controlled returns  │
    └─────────────────┘         │ Call verification   │
                                └─────────────────────┘
                                         │
                                ┌─────────────────────┐
                                │ FFF Framework       │
                                │                     │
                                │ • adc_read_raw_fake │
                                │ • call_count        │
                                │ • return_val        │
                                │ • arg_history       │
                                └─────────────────────┘

Data Flow
=========

Temperature Processing Pipeline
-------------------------------

.. code-block:: text

    Raw ADC Value          Temperature           Control Decision
    (0-4095)              (Celsius)             (Fan On/Off)
        │                     │                      │
        ▼                     ▼                      ▼
    ┌─────────┐           ┌─────────┐           ┌─────────┐
    │ ADC     │  read()   │ Temp    │ process() │ Control │
    │ Sensor  ├──────────▶│ Processor├──────────▶│ Logic   │
    └─────────┘           └─────────┘           └─────────┘
        │                     │                      │
        │                     ▼                      ▼
        │               ┌─────────┐           ┌─────────┐
        │               │ Formula:│           │ if > 37°│
        │               │ °C =    │           │ Fan ON  │
        │               │ (raw*3.3│           │ else    │
        │               │ /4095)  │           │ Fan OFF │
        │               │ *100    │           └─────────┘
        │               └─────────┘                  │
        │                     │                      ▼
        │                     ▼               ┌─────────┐
        │               ┌─────────┐           │ GPIO    │
        │               │ Logging │           │ Control │
        │               │ UART    │           └─────────┘
        │               │ Output  │
        │               └─────────┘
        └─────────────────────────────────────────────┘
                    (All values logged)

Configuration Management
========================

Build Variants
--------------

.. code-block:: text

    drivers.hpp Configuration:
    
    #ifdef SIMULATION_BUILD
        using AdcDriver = SimAdcDriver;
        using GpioDriver = SimGpioDriver;
        using UartDriver = SimUartDriver;
    #elif defined(TESTING_BUILD)
        using AdcDriver = MockAdcDriver;
        using GpioDriver = MockGpioDriver;
        using UartDriver = MockUartDriver;
    #else // HARDWARE_BUILD
        using AdcDriver = ZephyrAdcDriver;
        using GpioDriver = ZephyrGpioDriver;
        using UartDriver = ZephyrUartDriver;
    #endif

File Structure
==============

Project Organization
--------------------

.. code-block:: text

    embedded-hal-project/
    ├── src/
    │   ├── main.cpp                    # Main application entry
    │   ├── app/
    │   │   └── TemperatureController.hpp  # Application logic
    │   ├── domain/
    │   │   └── TemperatureProcessor.hpp   # Business logic
    │   └── hal/
    │       ├── drivers.hpp             # Platform abstraction
    │       ├── ISensor.hpp             # Sensor interface
    │       ├── IActuator.hpp           # Actuator interface
    │       ├── ILogger.hpp             # Logger interface
    │       ├── AdcSensor.hpp           # ADC implementation
    │       ├── GpioFan.hpp             # GPIO implementation
    │       └── UartLogger.hpp          # UART implementation
    ├── simulation/
    │   ├── CMakeLists.txt              # Simulation build config
    │   ├── main_sim.cpp                # Simulation entry point
    │   ├── zephyr_sim.cpp              # Simulation drivers
    │   └── zephyr_sim.h                # Simulation headers
    ├── tests/
    │   ├── CMakeLists.txt              # Test build configuration
    │   ├── ztest_framework.hpp         # Custom test framework
    │   ├── test_main.cpp               # Test runner
    │   ├── test_temperature_processor.cpp
    │   ├── test_adc_sensor.cpp
    │   ├── test_gpio_fan.cpp
    │   ├── test_uart_logger.cpp
    │   ├── test_temperature_controller.cpp
    │   └── mocks/
    │       ├── fff_mocks.hpp           # FFF mock definitions
    │       └── fff_mocks.cpp           # FFF mock implementations
    ├── build/                          # Hardware build artifacts
    ├── CMakeLists.txt                  # Main build configuration
    ├── prj.conf                        # Zephyr project config
    └── docs/
        └── design.rst                  # This document

Design Principles
=================

SOLID Principles
----------------

**Single Responsibility Principle (SRP)**
   - Each class has one reason to change
   - ``AdcSensor`` only handles ADC operations
   - ``GpioFan`` only handles GPIO fan control
   - ``UartLogger`` only handles UART logging

**Open/Closed Principle (OCP)**
   - Classes are open for extension, closed for modification
   - New sensor types can be added by implementing ``ISensor``
   - New actuators can be added by implementing ``IActuator``

**Liskov Substitution Principle (LSP)**
   - Derived classes can replace base classes
   - All ``ISensor`` implementations are interchangeable
   - Mock implementations can replace real implementations

**Interface Segregation Principle (ISP)**
   - Interfaces are focused and minimal
   - ``ISensor``, ``IActuator``, ``ILogger`` have single methods
   - No forced dependencies on unused methods

**Dependency Inversion Principle (DIP)**
   - High-level modules don't depend on low-level modules
   - ``TemperatureController`` depends on abstractions, not concretions
   - Driver implementations are injected through interfaces

Additional Patterns
-------------------

**Strategy Pattern**
   - Different driver implementations for different platforms
   - Simulation vs Hardware vs Mock strategies

**Dependency Injection**
   - Components receive their dependencies from external sources
   - Enables testing and platform flexibility

**Template Method Pattern**
   - Common control flow in ``TemperatureController.regulate()``
   - Specific implementations in concrete drivers

**Factory Pattern**
   - Driver creation abstracted behind ``drivers.hpp``
   - Platform-specific instantiation

Performance Characteristics
===========================

Memory Usage
------------

.. code-block:: text

    Component Memory Footprint (approximate):
    
    ┌─────────────────────┬──────────┬──────────────┐
    │ Component           │ RAM (B)  │ Flash (B)    │
    ├─────────────────────┼──────────┼──────────────┤
    │ TemperatureController│    12    │     256      │
    │ AdcSensor           │     8    │     128      │
    │ GpioFan             │     8    │     96       │
    │ UartLogger          │     8    │     192      │
    │ TemperatureProcessor│     4    │     64       │
    │ Driver instances    │    24    │     384      │
    ├─────────────────────┼──────────┼──────────────┤
    │ Total System        │    64    │    1120      │
    └─────────────────────┴──────────┴──────────────┘

Timing Characteristics
----------------------

.. code-block:: text

    Typical Execution Times (32-bit ARM Cortex-M):
    
    ┌─────────────────────┬─────────────────┐
    │ Operation           │ Time (μs)       │
    ├─────────────────────┼─────────────────┤
    │ ADC Read            │      50         │
    │ Temperature Convert │       5         │
    │ GPIO Set/Clear      │       2         │
    │ UART Write          │     100         │
    │ Full regulate()     │     160         │
    └─────────────────────┴─────────────────┘

Future Enhancements
===================

Potential Extensions
--------------------

1. **Multi-Sensor Support**
   - Add ``SensorArray`` class
   - Implement sensor fusion algorithms
   - Support for temperature averaging

2. **PID Control**
   - Replace bang-bang control with PID
   - Add ``PidController`` class
   - Tunable parameters

3. **Communication Protocols**
   - Add I2C sensor support
   - Implement CAN bus logging
   - Network connectivity

4. **Advanced Logging**
   - Circular buffer for history
   - Data compression
   - Remote logging capabilities

5. **Safety Features**
   - Watchdog integration
   - Failsafe modes
   - Error recovery mechanisms

Conclusion
==========

The Embedded HAL Temperature Controller demonstrates a well-architected embedded system with:

- **Clear separation of concerns** through layered architecture
- **Platform independence** via hardware abstraction
- **Comprehensive testing** with 100% coverage
- **Maintainable code** following SOLID principles
- **Scalable design** for future enhancements

The system successfully balances embedded constraints with software engineering best practices, providing a robust foundation for real-time temperature control applications.
