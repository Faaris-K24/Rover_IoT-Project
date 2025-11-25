#pragma once
#include <uart_message_transmitter.h>
#include "message_api.h"
#include <cstring> // for memcpy

template <typename T>
bool transmitMessage(MessageTransmitter &mt, sensor_id_t sensor_id, message_id msg_id, const T &data)
{
    uint8_t buf[MessageTransmitter::MAX_NUM_PACKETS];

    size_t headerSize = sizeof(msg_header);
    size_t payloadSize = sizeof(T);

    if (headerSize + payloadSize > MessageTransmitter::MAX_NUM_PACKETS) {
        return false; // too big
    }

    msg_header header;
    header.sensor_id = sensor_id;
    header.msg_id = msg_id;
    memcpy(buf, &header, headerSize);

    memcpy(buf + headerSize, &data, payloadSize);

    return mt.transmit((const char*)buf, headerSize + payloadSize);
}
