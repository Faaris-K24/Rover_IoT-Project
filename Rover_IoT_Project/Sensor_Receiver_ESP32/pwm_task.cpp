#include "pwm_task.h"

// Channels
constexpr int SERVO_CHANNEL = 0;
constexpr int MOTOR_CHANNEL = 1;

// Frequency and resolution
constexpr int PWM_FREQ = 100;
constexpr int PWM_RESOLUTION = 16;   // 0–65535 duty steps

SemaphoreHandle_t pwm_sem;

// Convert percent (0–100) to driver duty (0–65535)
static uint32_t convertDuty(float percent) {
    if (percent < 0.0f)   percent = 0.0f;
    if (percent > 100.0f) percent = 100.0f;

    return (uint32_t)((percent / 100.0f) * ((1 << PWM_RESOLUTION) - 1));
}

// Pin → channel lookup
static int getChannelFromPin(uint8_t pin) {
    if (pin == SERVO_PIN) return SERVO_CHANNEL;
    if (pin == MOTOR_PIN) return MOTOR_CHANNEL;
    return -1;
}

// Initialize both PWM outputs
static void PWMInit() {
    ledcAttachChannel(SERVO_PIN, PWM_FREQ, PWM_RESOLUTION, SERVO_CHANNEL);
    ledcWriteChannel(SERVO_CHANNEL, 0);

    ledcAttachChannel(MOTOR_PIN, PWM_FREQ, PWM_RESOLUTION, MOTOR_CHANNEL);
    ledcWriteChannel(MOTOR_CHANNEL, 0);
}

/* ------------------------- Public API ------------------------- */

void PWMSetDuty(uint8_t pin, float duty_percent) {
    int channel = getChannelFromPin(pin);
    if (channel < 0) return;

    uint32_t duty = convertDuty(duty_percent);
    ledcWriteChannel(channel, duty);
}

void PWMTask(void *pvParameters) {
    (void) pvParameters;

    PWMInit();

    PWMSetDuty(SERVO_PIN, 15.48f); // idle
    PWMSetDuty(MOTOR_PIN, 15.02f); // idle
    //PWMSetDuty(SERVO_PIN, 10.24f);

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
