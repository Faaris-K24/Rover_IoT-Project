#pragma once
#include <Arduino.h>

// PWM pins
#define SERVO_PIN 25
#define MOTOR_PIN 26

// Set PWM duty cycle for a given pin
// duty_percent = 0.0f to 100.0f (percentage)
void PWMSetDuty(uint8_t pin, float duty_percent);

// FreeRTOS task that initializes PWM
void PWMTask(void *pvParameters);
