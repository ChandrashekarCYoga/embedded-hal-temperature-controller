#include "ztest_framework.hpp"
#include "mocks/fff_mocks.hpp"
#include "UartLogger.hpp"
#include <cstring>
#include <string>

// Define MockUartDriver as UartDriver for testing
#define UartDriver MockUartDriver

ZTEST(uart_logger, test_logs_temperature_value) {
    reset_all_fakes();
    MockUartDriver mock_uart;
    UartLogger logger(mock_uart);
    
    logger.log(25.5f);
    
    // Verify UART write was called
    zassert_equal(uart_write_fake.call_count, 1, "Should call uart_write once");
    
    // Check the captured message
    auto messages = mock_uart.getMessages();
    zassert_equal(messages.size(), 1, "Should have captured one message");
    
    std::string message = messages[0];
    zassert_true(message.find("25.50") != std::string::npos, 
                ("Should contain temperature value in message. Got: " + message).c_str());
}

ZTEST(uart_logger, test_logs_multiple_values) {
    reset_all_fakes();
    MockUartDriver mock_uart;
    UartLogger logger(mock_uart);
    
    float temperatures[] = {20.0f, 25.5f, 30.25f, 37.0f};
    
    for (int i = 0; i < 4; i++) {
        logger.log(temperatures[i]);
    }
    
    zassert_equal(uart_write_fake.call_count, 4, "Should call uart_write four times");
}

ZTEST(uart_logger, test_polymorphic_behavior) {
    reset_all_fakes();
    MockUartDriver mock_uart;
    UartLogger logger(mock_uart);
    
    // Test polymorphic behavior
    ILogger* base_logger = &logger;
    base_logger->log(42.0f);
    
    zassert_equal(uart_write_fake.call_count, 1, "Should work through ILogger interface");
    
    // Check the captured message
    auto messages = mock_uart.getMessages();
    zassert_equal(messages.size(), 1, "Should have captured one message");
    
    std::string message = messages[0];
    zassert_true(message.find("42.00") != std::string::npos, 
                ("Should contain temperature value in message. Got: " + message).c_str());
}
