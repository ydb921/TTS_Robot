#ifndef __BSP_PID_H
#define    __BSP_PID_H

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float target_val;   //Ŀ��ֵ
    float actual_val;   //ʵ��ֵ
    float err;          //����ƫ��ֵ
    float err_last;     //������һ��ƫ��ֵ
    float Kp, Ki, Kd;   //������������֡�΢��ϵ��
    float integral;     //�������ֵ
} _pid;

float pid_realize(_pid *pid, float actual_val, void(*function)(_pid *pid));

void pid_init(_pid *pid, float Kp, float Ki, float Kd);

void pid_speed_ead(_pid *pid);

void pid_location_ead(_pid *pid);
void pid_NULL(_pid *pid);
#endif
