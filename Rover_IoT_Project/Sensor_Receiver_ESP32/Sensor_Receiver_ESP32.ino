#include <Arduino.h>
#include <uart_message_transmitter.h>
#include <message_api.h>
#include "message_processor.h"
#include "mqtt_publisher.h"
#include "pwm_task.h"

MessageTransmitter message_if;

// Optional task handle
TaskHandle_t messageTaskHandle = NULL;
TaskHandle_t mqttTaskHandle = NULL;
TaskHandle_t pwmTaskHandle = NULL;

// -------------------- Message Task --------------------
void MessageTask(void *pvParameters) {
    (void)pvParameters;

    char buf[128] = {0};
    size_t bytes_received = 0;
    bool verbose = false;

    for (;;) {
        if (message_if.receive(buf, bytes_received)) {
            // Process message (verbose mode)
            processMessage(buf, bytes_received, verbose);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


void MQTTPublishTask(void *pvParameters) {
    for (;;) {
        MQTTPublisher::publishSensorData();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void MQTTLoopTask(void *pvParameters) {
    for (;;) {
        MQTTPublisher::loop();  
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


void PWMTaskWrapper(void *pvParameters)
{
    PWMTask(pvParameters);
    /* Never return. */
}

void setup() {
    Serial.begin(115200);
    message_if.init(Serial2, 115200, SERIAL_8N1, 16, 17); // RX=16, TX=17
    Serial.println("Setup Done");

    MQTTPublisher::setup("SamsungS25U-F", "xxxpasswordxxx",
                         "broker.hivemq.com", 1883);

    xTaskCreate(MessageTask, "MessageTask", 4096, NULL, 2, &messageTaskHandle);
    xTaskCreate(MQTTLoopTask,"MQTT Loop", 4096, NULL, 1, NULL);
    xTaskCreate(MQTTPublishTask, "MQTT Publish", 4096, NULL, 1, NULL );
    xTaskCreate(PWMTaskWrapper, "PWMTask", 2048, NULL, 1, &pwmTaskHandle);
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
