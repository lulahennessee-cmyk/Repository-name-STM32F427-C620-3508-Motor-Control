#include "control.h"
#include "motor.h"


PID_t speed_pid;

PID_t angle_pid;

void Motor_ControlInit(void)
{
    PID_Init(
        &speed_pid,
        1.0f,
        0.0f,
        0.0f,
        5000.0f,
        3000.0f
    );
    PID_Init(
        &angle_pid,
        5.0f,
        0.0f,
        0.0f,
        2000.0f,
        1000.0f
    );
}

void Motor_SpeedControl(float target_speed)
{
    float output;
	
    output = PID_Calc(
        &speed_pid,
        target_speed,
        motor1.speed_rpm
    );

    Motor_SendCurrent((int16_t)output);
}

void Motor_PositionControl(float target_angle)
{
    float actual_angle;
    float target_speed;

    actual_angle = Motor_GetAngle();

    target_speed = PID_Calc(
        &angle_pid,
        target_angle,
        actual_angle
    );

    Motor_SpeedControl(target_speed);
}
