#pragma once
class IActuator {
public:
    virtual ~IActuator() = default;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
};
