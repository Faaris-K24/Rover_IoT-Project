#include "uart_message_transmitter.h"
#include <Arduino.h>

MessageTransmitter::MessageTransmitter()
    : serial(nullptr), rxState(WAITING_START), packetSize(0), receivedSoFar(0)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));
}

// --- Basic init: use default serial pin mapping ---
bool MessageTransmitter::init(HardwareSerial &serialPort, unsigned long baud) {
    serial = &serialPort;
    serial->begin(baud);
    rxState = WAITING_START;
    packetSize = 0;
    receivedSoFar = 0;
    memset(rxBuffer, 0, sizeof(rxBuffer));
    return true;
}

// --- Advanced init: specify UART config + RX/TX pin mapping ---
bool MessageTransmitter::init(HardwareSerial &serialPort, unsigned long baud,
                              uint32_t config, int8_t rxPin, int8_t txPin) {
    serial = &serialPort;
#if defined(ARDUINO_ARCH_ESP32)
    serial->begin(baud, config, rxPin, txPin);
#else
    serial->begin(baud);
#endif
    rxState = WAITING_START;
    packetSize = 0;
    receivedSoFar = 0;
    memset(rxBuffer, 0, sizeof(rxBuffer));
    return true;
}


bool MessageTransmitter::transmit(const char *buf, uint8_t size) {
    if (serial == nullptr || buf == nullptr || size == 0) {
        return false;
    }

    serial->write(START_BYTE);
    serial->write(size);
    serial->write((const uint8_t*)buf, size);
    serial->flush();
    return true;
}

bool MessageTransmitter::receive(char *buf, size_t &bytes_received) {
    if (serial == nullptr) return false;

    bytes_received = 0;

    while (serial->available()) {
        uint8_t byte = serial->read();

        switch (rxState) {
            case WAITING_START:
                if (byte == START_BYTE) {
                    rxState = WAITING_SIZE;
                }
                break;

            case WAITING_SIZE:
                packetSize = byte;
                if (packetSize == 0 || packetSize > MAX_NUM_PACKETS) {
                    rxState = WAITING_START;
                    receivedSoFar = 0;
                    break;
                }
                receivedSoFar = 0;
                rxState = WAITING_PAYLOAD;
                break;

            case WAITING_PAYLOAD:
                if (receivedSoFar < MAX_NUM_PACKETS) {
                    rxBuffer[receivedSoFar++] = byte;
                    if (receivedSoFar == packetSize) {
                        memcpy(buf, rxBuffer, packetSize);
                        bytes_received = packetSize;

                        rxState = WAITING_START;
                        receivedSoFar = 0;
                        packetSize = 0;
                        return true;
                    }
                } else {
                    rxState = WAITING_START;
                    receivedSoFar = 0;
                    packetSize = 0;
                    return false;
                }
                break;
        }
    }

    return false;
}
