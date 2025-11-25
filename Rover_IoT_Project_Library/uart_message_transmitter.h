#pragma once

#include <Arduino.h>

#define START_BYTE        0xAA

class MessageTransmitter {
public:
    enum RxState {
        WAITING_START,
        WAITING_SIZE,
        WAITING_PAYLOAD
    };

    MessageTransmitter();                       // Default constructor
    // Basic init (just baud)
    bool init(HardwareSerial &serialPort, unsigned long baud);
    // Overloaded init with pin + config options
    bool init(HardwareSerial &serialPort, unsigned long baud,
              uint32_t config, int8_t rxPin, int8_t txPin);
    bool transmit(const char *buf, uint8_t size);
    bool receive(char *buf, size_t &bytes_received);
    static constexpr uint8_t MAX_NUM_PACKETS = 254;
private:
    HardwareSerial *serial;                     // Pointer to serial port
    RxState rxState;
    uint8_t rxBuffer[MAX_NUM_PACKETS];
    uint8_t packetSize;
    uint8_t receivedSoFar;
};

