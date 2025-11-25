#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "message_processor.h"
#include "message_api.h"

class MQTTPublisher {
public:
    static void setup(const char* ssid, const char* password,
                      const char* broker, uint16_t port);

    static void publishSensorData();
    static void loop();
private:
    static void reconnect();

    static WiFiClient wifiClient;
    static PubSubClient mqttClient;

    // MQTT topics
    static constexpr const char* HTS_TOPIC = "sensors/htsf1";
    static constexpr const char* PRESSURE_TOPIC = "sensors/pressuref1";
    static constexpr const char* IMU_TOPIC = "sensors/imuf1";
};
