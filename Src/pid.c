#include "pid.h"


void PID_Init(PID_t *pid,
              float kp,
              float ki,
              float kd,
              float max_output,
              float max_integral)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;//remain PID parameter

    pid->target = 0;
    pid->measure = 0;

    pid->error = 0;
    pid->last_error = 0;

    pid->integral = 0;
    pid->output = 0;//initiate

    pid->max_output = max_output;
    pid->max_integral = max_integral;//remain limit
}


float PID_Calc(PID_t *pid,
               float target,
               float measure)
{
    float derivative; //Error variation

    pid->target = target;  //target
    pid->measure = measure;//real

    pid->error = target - measure;

    pid->integral += pid->error;//integral

    if(pid->integral > pid->max_integral)
    {
        pid->integral = pid->max_integral;
    }

    if(pid->integral < -pid->max_integral)
    {
        pid->integral = -pid->max_integral;
    }//limit

    derivative = pid->error - pid->last_error;//variation of error 

    pid->output =
        pid->kp * pid->error
        + pid->ki * pid->integral
        + pid->kd * derivative;

    if(pid->output > pid->max_output)
    {
        pid->output = pid->max_output;
    }

    if(pid->output < -pid->max_output)
    {
        pid->output = -pid->max_output;
    }//limit

    pid->last_error = pid->error;

    return pid->output;
}
