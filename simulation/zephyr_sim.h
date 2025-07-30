#pragma once
#include <cstdint>
#include <chrono>
#include <thread>
#include <cstdio>

// Simulate Zephyr's k_sleep function
#define K_SECONDS(s) (s * 1000)
inline void k_sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
