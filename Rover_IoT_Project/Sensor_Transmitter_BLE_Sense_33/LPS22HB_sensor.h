#pragma once

#include "sensor_abstract.h"
#include <Arduino_LPS22HB.h>

struct PressureData
{
    float pressure;
};


class PressureSensor : public Sensor {
public:
    bool init() override {
        is_initialized = BARO.begin();
        if (!is_initialized) {
            Serial.println("Failed to initialize LPS22HB!");
        }
        return is_initialized;
    }

    void read() override {
        if (!is_initialized) return;

        data.pressure = BARO.readPressure();

#ifdef ENABLE_LOGGING
        Serial.print("Pressure: ");
        Serial.print(data.pressure);
        Serial.println(" hPa");
#endif
    }
    PressureData getData()
    {
        return data;
    }
private:
    PressureData data;
};
