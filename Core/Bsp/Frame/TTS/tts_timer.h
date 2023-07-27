/**
 * @file system_tim.h
 * @author DB (ydbdby921@gmail.com)
 * @brief
 * @version 0.2
 * @date 2021-09-04
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __TTS_SYSTEM_TIM_H__
#define __TTS_SYSTEM_TIM_H__


typedef enum
{
    TTS_Timer_SUCCESS,
    TTS_Timer_FAIL,
} Timer_Result_TypeDef;

typedef __UINT16_TYPE__ Timer_Type;

typedef enum
{
    TTS_Timer_STOP,  //定时器停止
    TTS_Timer_START, //定时启动
} Timer_Status_TypeDef;

typedef struct
{
    Timer_Status_TypeDef Status;   // 0-定时器未启动 1-定时器已启动
    volatile Timer_Type CurrentCount; //当前计时值
    Timer_Type Period;       //定时周期
    void (*func)(void);          //函数指针
} Stu_TimerTypeDef;

typedef struct   //时间结构体
{
    __UINT32_TYPE__ Total;
    __UINT32_TYPE__ CurrentMillis;
    __UINT8_TYPE__ Seconds;
    __UINT8_TYPE__ Minutes;
    __UINT32_TYPE__ Hours;
} Time_TypeDef;

/**
 * 软件定时器
 * @param User_Timer 定时器句柄
 * @param User_Sum 句柄数量
 */
void TTS_TimerInit(Stu_TimerTypeDef *User_Timer, unsigned char User_Sum);

Timer_Status_TypeDef TTS_TimerCreation(unsigned char id, void (*Proc)(void),
                                       unsigned short Period, Timer_Status_TypeDef state);

Timer_Result_TypeDef TTS_ResetTimer(unsigned char id, Timer_Status_TypeDef sta);

void TTS_TimerHandle(void);

void TTS_Timer_WritePeriod(unsigned char id, unsigned short Period);

Timer_Type TTS_Timer_ReadPeriod(__UINT8_TYPE__ id);

void TTS_Timer_SetCount(unsigned char id, unsigned int Count);

void TTS_CurrentTimeHandle(Time_TypeDef *User_Time, __UINT16_TYPE__ Add);

Timer_Result_TypeDef TTS_DeleteTimer(unsigned char id);

#endif /* __TTS_SYSTEM_TIM_H__ */
