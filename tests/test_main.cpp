#include "ztest_framework.hpp"

// Global test case storage
std::vector<TestCase> test_cases;

void register_test(const std::string& suite, const std::string& name, std::function<void()> func) {
    test_cases.push_back({name, func, suite});
}

// Forward declarations for test functions
extern "C" {
    void setUp();
    void tearDown();
}

int main() {
    std::cout << "Running embedded HAL unit tests...\n" << std::endl;
    
    int passed = 0;
    int failed = 0;
    
    for (const auto& test : test_cases) {
        std::cout << "Running " << test.suite << "::" << test.name << "... ";
        
        try {
            setUp();
            test.test_func();
            tearDown();
            std::cout << "PASS" << std::endl;
            passed++;
        } catch (const std::exception& e) {
            std::cout << "FAIL" << std::endl;
            std::cout << "  " << e.what() << std::endl;
            failed++;
        }
    }
    
    std::cout << "\nTest Results:" << std::endl;
    std::cout << "  Passed: " << passed << std::endl;
    std::cout << "  Failed: " << failed << std::endl;
    std::cout << "  Total:  " << (passed + failed) << std::endl;
    
    return failed == 0 ? 0 : 1;
}

// Global setup/teardown functions
extern "C" {
    void setUp() {
        // Reset FFF mocks before each test
        extern void reset_all_mocks();
        reset_all_mocks();
    }
    
    void tearDown() {
        // Cleanup after each test if needed
    }
}
