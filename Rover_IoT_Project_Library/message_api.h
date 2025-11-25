#pragma once

enum sensor_id_t : uint8_t
{
    HTS_SENSOR_ID,
    LPS22HB_SENSOR_ID,
    LSM9DS1_SENSOR_ID,
};

enum message_id : uint8_t
{
    DATA_EVENT,
};

struct __attribute__((packed)) msg_header
{
    sensor_id_t sensor_id; // 1 byte
    message_id msg_id;      // 1 byte
};

// Payload Structs

struct HTSData_msg
{
    float temp;
    float hum;
};


struct PressureData_msg
{
    float pressure;
};


struct IMUData_msg
{
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
};