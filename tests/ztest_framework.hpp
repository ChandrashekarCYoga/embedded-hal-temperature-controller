#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cmath>

// Simple test framework (since we're in simulation environment, not full Zephyr)
struct TestCase {
    std::string name;
    std::function<void()> test_func;
    std::string suite;
};

extern std::vector<TestCase> test_cases;

void register_test(const std::string& suite, const std::string& name, std::function<void()> func);

#define ZTEST(suite, test) \
    static void suite##_##test(); \
    static int suite##_##test##_register = (register_test(#suite, #test, suite##_##test), 0); \
    static void suite##_##test()

#define zassert_true(condition, msg) \
    if (!(condition)) { \
        throw std::runtime_error(std::string("Assertion failed: ") + msg); \
    }

#define zassert_false(condition, msg) \
    if (condition) { \
        throw std::runtime_error(std::string("Assertion failed: ") + msg); \
    }

#define zassert_equal(a, b, msg) \
    if ((a) != (b)) { \
        throw std::runtime_error(std::string("Assertion failed: ") + msg + " (expected " + std::to_string(b) + ", got " + std::to_string(a) + ")"); \
    }

#define zassert_float_equal(a, b, msg) \
    if (std::abs((a) - (b)) > 0.1f) { \
        throw std::runtime_error(std::string("Assertion failed: ") + msg + " (expected " + std::to_string(b) + ", got " + std::to_string(a) + ")"); \
    }

#define zassert_not_null(ptr, msg) \
    if ((ptr) == nullptr) { \
        throw std::runtime_error(std::string("Assertion failed: ") + msg); \
    }

#define zassert_str_equal(a, b, msg) \
    if (std::string(a) != std::string(b)) { \
        throw std::runtime_error(std::string("Assertion failed: ") + msg + " (expected '" + std::string(b) + "', got '" + std::string(a) + "')"); \
    }

#define zassert_str_not_equal(a, b, msg) \
    if (std::string(a) == std::string(b)) { \
        throw std::runtime_error(std::string("Assertion failed: ") + msg + " (both strings are '" + std::string(a) + "')"); \
    }
