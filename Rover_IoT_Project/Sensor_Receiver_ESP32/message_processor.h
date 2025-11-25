#pragma once
#include <Arduino.h>
#include <message_api.h>

// Processes a received UART message buffer
// verbose = 1 -> print decoded data
// verbose = 0 -> silent (store only)
void processMessage(const char* buf, size_t bytes_received, bool verbose = false);

// Getter functions for latest received data
bool getLatestHTSData(HTSData_msg &out);
bool getLatestPressureData(PressureData_msg &out);
bool getLatestIMUData(IMUData_msg &out);
