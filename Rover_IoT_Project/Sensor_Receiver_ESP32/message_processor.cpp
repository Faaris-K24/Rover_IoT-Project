#include "message_processor.h"

// Static storage for latest sensor data
static HTSData_msg latestHTSData;
static PressureData_msg latestPressureData;
static IMUData_msg latestIMUData;

// Flags to indicate valid data
static bool hasHTSData = false;
static bool hasPressureData = false;
static bool hasIMUData = false;

void processMessage(const char* buf, size_t bytes_received, bool verbose)
{
    if (bytes_received < sizeof(msg_header)) {
        if (verbose) Serial.println("Packet too small");
        return;
    }

    msg_header header;
    memcpy(&header, buf, sizeof(msg_header));

    switch (header.sensor_id) {
        case HTS_SENSOR_ID: {
            if (header.msg_id == DATA_EVENT &&
                bytes_received == sizeof(msg_header) + sizeof(HTSData_msg)) {
                
                memcpy(&latestHTSData, buf + sizeof(msg_header), sizeof(latestHTSData));
                hasHTSData = true;

                if (verbose) {
                    Serial.print("HTS -> Temp: ");
                    Serial.print(latestHTSData.temp);
                    Serial.print(" C, Hum: ");
                    Serial.print(latestHTSData.hum);
                    Serial.println(" %");
                }
            } else if (verbose) {
                Serial.println("Incorrect HTS payload size");
            }
            break;
        }

        case LPS22HB_SENSOR_ID: {
            if (header.msg_id == DATA_EVENT &&
                bytes_received == sizeof(msg_header) + sizeof(PressureData_msg)) {
                
                memcpy(&latestPressureData, buf + sizeof(msg_header), sizeof(latestPressureData));
                hasPressureData = true;

                if (verbose) {
                    Serial.print("Pressure: ");
                    Serial.print(latestPressureData.pressure);
                    Serial.println(" hPa");
                }
            } else if (verbose) {
                Serial.println("Incorrect Pressure payload size");
            }
            break;
        }

        case LSM9DS1_SENSOR_ID: {
            if (header.msg_id == DATA_EVENT &&
                bytes_received == sizeof(msg_header) + sizeof(IMUData_msg)) {
                
                memcpy(&latestIMUData, buf + sizeof(msg_header), sizeof(latestIMUData));
                hasIMUData = true;

                if (verbose) {
                    Serial.print("IMU -> Accel: (");
                    Serial.print(latestIMUData.ax); Serial.print(", ");
                    Serial.print(latestIMUData.ay); Serial.print(", ");
                    Serial.print(latestIMUData.az); Serial.print(") ");

                    Serial.print("Gyro: (");
                    Serial.print(latestIMUData.gx); Serial.print(", ");
                    Serial.print(latestIMUData.gy); Serial.print(", ");
                    Serial.print(latestIMUData.gz); Serial.print(") ");

                    Serial.print("Mag: (");
                    Serial.print(latestIMUData.mx); Serial.print(", ");
                    Serial.print(latestIMUData.my); Serial.print(", ");
                    Serial.print(latestIMUData.mz); Serial.println(")");
                }
            } else if (verbose) {
                Serial.println("Incorrect IMU payload size");
            }
            break;
        }

        default:
            if (verbose) {
                Serial.print("Unknown sensor ID: ");
                Serial.println(header.sensor_id);
            }
            break;
    }
}

// --- Getters ---

bool getLatestHTSData(HTSData_msg &out) {
    if (!hasHTSData) return false;
    out = latestHTSData;
    return true;
}

bool getLatestPressureData(PressureData_msg &out) {
    if (!hasPressureData) return false;
    out = latestPressureData;
    return true;
}

bool getLatestIMUData(IMUData_msg &out) {
    if (!hasIMUData) return false;
    out = latestIMUData;
    return true;
}
