#pragma once

// ILogger optional for DIP
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(float val) = 0;
};