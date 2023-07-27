//
// Created by dby on 2023/7/12.
//

#ifndef USER_H
#define USER_H


#include "main.h"
#include "../Bsp/Show_Class/bsp_pin.h"
#include "tim.h"
#define User_Timer_Tim htim6

/* ºËÐÄÈÎÎñ¾ä±ú */
typedef enum
{
    OS_TASK_Pin,
    OS_TASK_Queue,
    OS_TASK_Button,
    OS_TASK_Proc,
    OS_TASK_SUM,
} OS_TaskID_t;

typedef enum
{
    OS_Timer_Time,
    OS_MPU_Clock,
    OS_Motor_Clock,
    OS_Timer_Clock,
    OS_Timer_SUM,
} OS_TimerID_t;

/**
 * @brief Task Start
 *
 */
void User_Create_task(void);
/**
 * @brief Handle the underlying architecture creation
 *
 */
void User_Bsp_Init(void);

void Spcp_Callback(uint8_t ID, uSPCPData_t *Data, uint8_t Length, const uint8_t *FrameData);

#endif //USER_H
