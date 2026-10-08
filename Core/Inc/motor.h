#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"

typedef struct
{
    uint16_t ecd;       //The value of ecd 0~8191
    int16_t speed_rpm;  //The speed returned by C620
    int16_t current;    //The current returned by C620
    uint8_t temperature;//T

    uint16_t last_ecd;  //last the value of ecd-->To now-last
    int32_t total_ecd;  //accumulative ecd
    uint8_t ecd_init;   //initiate ecd--first dont have

} Motor_t;

extern Motor_t motor1;

void Motor_Update(uint8_t data[8]);  //Analyze the date sent from C620
void Motor_SendCurrent(int16_t current);  //Sent PIDs output by the from of current,transform current to CAN language

float Motor_GetAngle(void);    //culculate value of angle by value of ecd

#endif
