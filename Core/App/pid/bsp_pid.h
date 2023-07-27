#ifndef __BSP_PID_H
#define    __BSP_PID_H

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float target_val;   //目标值
    float actual_val;   //实际值
    float err;          //定义偏差值
    float err_last;     //定义上一个偏差值
    float Kp, Ki, Kd;   //定义比例、积分、微分系数
    float integral;     //定义积分值
} _pid;

float pid_realize(_pid *pid, float actual_val, void(*function)(_pid *pid));

void pid_init(_pid *pid, float Kp, float Ki, float Kd);

void pid_speed_ead(_pid *pid);

void pid_location_ead(_pid *pid);
void pid_NULL(_pid *pid);
#endif
