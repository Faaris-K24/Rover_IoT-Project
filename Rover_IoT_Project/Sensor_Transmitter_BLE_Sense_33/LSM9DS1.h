#pragma once

#include "sensor_abstract.h"
#include <Arduino_LSM9DS1.h>

struct IMUData
{
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
};

class IMUSensor : public Sensor {
public:
    bool init() override {
        is_initialized = IMU.begin();
        if (!is_initialized) {
            Serial.println("Failed to initialize LSM9DS1!");
        }
        return is_initialized;
    }

    void read() override {
        if (!is_initialized) return;

        if (IMU.accelerationAvailable()) {
            IMU.readAcceleration(data.ax, data.ay, data.az);
        }

        if (IMU.gyroscopeAvailable()) {
            IMU.readGyroscope(data.gx, data.gy, data.gz);
        }

        if (IMU.magneticFieldAvailable()) {
            IMU.readMagneticField(data.mx, data.my, data.mz);
        }

#ifdef ENABLE_LOGGING
        Serial.print("Accel: ");
        Serial.print(data.ax); Serial.print(", ");
        Serial.print(data.ay); Serial.print(", ");
        Serial.print(data.az); Serial.print(" m/s² | ");

        Serial.print("Gyro: ");
        Serial.print(data.gx); Serial.print(", ");
        Serial.print(data.gy); Serial.print(", ");
        Serial.print(data.gz); Serial.print(" °/s | ");

        Serial.print("Mag: ");
        Serial.print(data.mx); Serial.print(", ");
        Serial.print(data.my); Serial.print(", ");
        Serial.print(data.mz); Serial.println(" uT");
#endif
    }

    IMUData getData() {
        return data;
    }

private:
    IMUData data;
};
