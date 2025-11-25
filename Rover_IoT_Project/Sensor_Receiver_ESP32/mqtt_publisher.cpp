#include "mqtt_publisher.h"
#include "drive_cmd.h"

WiFiClient MQTTPublisher::wifiClient;
PubSubClient MQTTPublisher::mqttClient(wifiClient);

constexpr const char* DRIVE_COMMAND_TOPIC = "/rovdashboard/drivecmd";

// Maps a byte value to drive_command_t
drive_command_t byteToDriveCommand(uint8_t b) {
    switch (b) {
        case 0: return GO;
        case 1: return LEFT;
        case 2: return RIGHT;
        case 3: return STRAIGHT;
        case 4: return STOP;
        default: return STOP; 
    }
}


void rxCallback(char* topic, byte* payload, unsigned int length) {
    if (length < 1) return;

    uint8_t cmdNum = payload[0] - '0';

    Serial.print("RX cmd: ");
    Serial.println(cmdNum);

    driver_set_command(byteToDriveCommand(cmdNum));
}
void MQTTPublisher::setup(const char* ssid, const char* password,
                          const char* broker, uint16_t port) {
    Serial.println("[MQTT] Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[MQTT] WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    mqttClient.setServer(broker, port);
    mqttClient.setCallback(rxCallback);
    reconnect();
}

void MQTTPublisher::reconnect() {
    while (!mqttClient.connected()) {
        Serial.print("[MQTT] Attempting connection...");
        if (mqttClient.connect("ESP32Clientfk212")) {
            Serial.println(" connected.");
            mqttClient.subscribe(DRIVE_COMMAND_TOPIC);
        } else {
            Serial.print(" failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" retrying in 2 seconds...");
            delay(2000);
        }
    }
}

void MQTTPublisher::loop() {
    if (!mqttClient.connected()) {
        reconnect();
    }
    mqttClient.loop();
}

void MQTTPublisher::publishSensorData() {
  if (!mqttClient.connected()) {
      reconnect();
  }
  
  HTSData_msg hts;
  PressureData_msg pres;
  IMUData_msg imu;

  char payload[256];

  if (getLatestHTSData(hts)) {
    snprintf(payload, sizeof(payload),
              "{\"temp\":%.2f,\"hum\":%.2f}", hts.temp, hts.hum);
    if(!mqttClient.publish(HTS_TOPIC, payload))
    {
      Serial.println("Error0");
    }
    //Serial.print("[MQTT] Published HTS: "); Serial.println(payload);
  }
  else
  {
    Serial.println("no data");
  }

  if (getLatestPressureData(pres)) {
    snprintf(payload, sizeof(payload),
              "{\"pressure\":%.2f}", pres.pressure);
    if(!mqttClient.publish(PRESSURE_TOPIC, payload))
    {
      Serial.println("Error1");
    }
    //Serial.print("[MQTT] Published Pressure: "); Serial.println(payload);
  }

  if (getLatestIMUData(imu)) {
    snprintf(payload, sizeof(payload),
              "{\"ax\":%.2f,\"ay\":%.2f,\"az\":%.2f,"
              "\"gx\":%.2f,\"gy\":%.2f,\"gz\":%.2f,"
              "\"mx\":%.2f,\"my\":%.2f,\"mz\":%.2f}",
              imu.ax, imu.ay, imu.az,
              imu.gx, imu.gy, imu.gz,
              imu.mx, imu.my, imu.mz);
    if(!mqttClient.publish(IMU_TOPIC, payload))
    {
      Serial.println("Error2");
    }

    //Serial.print("[MQTT] Published IMU: "); Serial.println(payload);
  }
}
