#pragma once

#include "sensor_abstract.h"
#include <Arduino_HTS221.h>

//#define ENABLE_LOGGING

struct HTSData {
    float temp;
    float hum;
};


class HTSSensor : public Sensor {
public:
    bool init() override {
        is_initialized = HTS.begin();
        if (!is_initialized) {
            Serial.println("Failed to initialize HTS221!");
        }
        return is_initialized;
    }

    void read() override {
        if (!is_initialized) return;

        data.temp = HTS.readTemperature();
        data.hum = HTS.readHumidity();

#ifdef ENABLE_LOGGING
        Serial.print("Temp: "); 
        Serial.print(data.temp);
        Serial.print(" °C, Hum: "); 
        Serial.println(data.hum);
#endif

    }

    HTSData getData()
    {
        return data;
    }
private:
    HTSData data;
};
