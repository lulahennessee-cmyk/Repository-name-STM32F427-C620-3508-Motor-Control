#ifndef PID_H
#define PID_H

typedef struct
{
    float kp;
    float ki;
    float kd;

    float target;
    float measure;

    float error;
    float last_error;

    float integral;

    float output;

    float max_output;//limit output
    float max_integral;//limit integral 
	//to protect

} PID_t;

void PID_Init(PID_t *pid,//initiate
              float kp,
              float ki,
              float kd,
              float max_output,
              float max_integral);


float PID_Calc(PID_t *pid,
               float target,
               float measure);//

#endif
							 