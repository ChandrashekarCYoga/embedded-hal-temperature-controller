#pragma once
#include <cstdint>
#include <string>
#include <vector>

// Mock drivers for testing with controllable behavior
class MockAdcDriver {
private:
    uint16_t mock_value_;
    std::vector<uint16_t> value_sequence_;
    size_t sequence_index_;

public:
    MockAdcDriver() : mock_value_(800), sequence_index_(0) {}
    
    void setMockValue(uint16_t value) {
        mock_value_ = value;
    }
    
    void setValueSequence(const std::vector<uint16_t>& sequence) {
        value_sequence_ = sequence;
        sequence_index_ = 0;
    }
    
    uint16_t readRaw() {
        if (!value_sequence_.empty()) {
            uint16_t value = value_sequence_[sequence_index_];
            sequence_index_ = (sequence_index_ + 1) % value_sequence_.size();
            return value;
        }
        return mock_value_;
    }
};

class MockGpioDriver {
private:
    bool pin_state_;
    int high_count_;
    int low_count_;

public:
    MockGpioDriver() : pin_state_(false), high_count_(0), low_count_(0) {}
    
    void setHigh() {
        pin_state_ = true;
        high_count_++;
    }
    
    void setLow() {
        pin_state_ = false;
        low_count_++;
    }
    
    bool getState() const { return pin_state_; }
    int getHighCount() const { return high_count_; }
    int getLowCount() const { return low_count_; }
    
    void reset() {
        pin_state_ = false;
        high_count_ = 0;
        low_count_ = 0;
    }
};

class MockUartDriver {
private:
    std::vector<std::string> written_messages_;

public:
    void write(const char* msg) {
        written_messages_.push_back(std::string(msg));
    }
    
    const std::vector<std::string>& getWrittenMessages() const {
        return written_messages_;
    }
    
    std::string getLastMessage() const {
        return written_messages_.empty() ? "" : written_messages_.back();
    }
    
    size_t getMessageCount() const {
        return written_messages_.size();
    }
    
    void clear() {
        written_messages_.clear();
    }
};
