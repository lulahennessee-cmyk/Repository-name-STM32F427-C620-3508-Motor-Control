#ifndef CONTROL_H
#define CONTROL_H

#include "pid.h"

void Motor_ControlInit(void);

void Motor_SpeedControl(float target_speed);

void Motor_PositionControl(float target_angle);

#endif
