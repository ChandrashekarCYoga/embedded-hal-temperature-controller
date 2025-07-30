#pragma once
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual float readValue() = 0;
};
