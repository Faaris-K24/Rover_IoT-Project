#pragma once

#include "pwm_task.h"

enum drive_command_t
{
  GO = 0, //motor pwm = 15.8%
  LEFT,  //11.00%
  RIGHT,    //19.00%
  STRAIGHT, // servo pwm = 15.48
  STOP //motor pwm = 15.02
};


void driver_set_command(drive_command_t cmd);