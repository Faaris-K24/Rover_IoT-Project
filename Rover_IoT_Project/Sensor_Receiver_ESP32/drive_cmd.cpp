#include "drive_cmd.h"
#include "pwm_task.h" 

#include "Arduino.h"

void driver_set_command(drive_command_t cmd)
{
    switch (cmd)
    {
        case GO:
            // motor pwm = 15.8%
            PWMSetDuty(MOTOR_PIN, 15.8f);
            //Serial.println("GO speed");
            break;

        case LEFT:
            // motor pwm = 11.00%
            PWMSetDuty(SERVO_PIN, 11.0f);
            Serial.println("LEFT speed");
            break;

        case RIGHT:
            // motor pwm = 19.00%
            PWMSetDuty(SERVO_PIN, 19.0f);
            Serial.println("RIGHT speed");
            break;

        case STRAIGHT:
            // servo pwm = 15.48%
            PWMSetDuty(SERVO_PIN, 15.48f);
            Serial.println("STRAIGHT speed");
            break;

        case STOP:
            // motor pwm = 15.02%
            PWMSetDuty(MOTOR_PIN, 15.02f);
            Serial.println("STOP speed");
            break;

        default:
            break;
    }
}
