#include <Arduino.h>
#include <uart_message_transmitter.h>

#include "HTS221_sensor.h"
#include "LPS22HB_sensor.h"
#include "LSM9DS1.h"
#include "sensor_transmitter_util.h"

#define TRANSMITTER

HTSSensor ht_sensor;
PressureSensor pressure_sensor;
IMUSensor imu_sensor;

MessageTransmitter message_if;

void setup() {
    Serial.begin(115200);

    ht_sensor.init();
    pressure_sensor.init();
    imu_sensor.init();

    message_if.init(Serial1, 115200);
}

void loop() {
    // Read sensors
    ht_sensor.read();
    pressure_sensor.read();
    imu_sensor.read();

    // Transmit data
    transmitMessage(message_if, HTS_SENSOR_ID, DATA_EVENT, ht_sensor.getData());
    transmitMessage(message_if, LPS22HB_SENSOR_ID, DATA_EVENT, pressure_sensor.getData());
    transmitMessage(message_if, LSM9DS1_SENSOR_ID, DATA_EVENT, imu_sensor.getData());

    delay(1000); 
}
