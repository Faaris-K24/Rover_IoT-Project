#pragma once
#include <Arduino.h>

class Sensor {
public:
    Sensor() : is_initialized(false) {}
    virtual ~Sensor() {}

    // Initialize the sensor
    virtual bool init() = 0;

    // Read sensor data
    virtual void read() = 0;

protected:
    bool is_initialized;
};
