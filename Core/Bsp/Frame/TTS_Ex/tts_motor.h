#ifndef _TTS_MOTOR_H
#define _TTS_MOTOR_H
/**
 * @file tts_motor.c
 * @author DB (ydbdby921@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdint.h>

typedef enum
{
    Motor_ENABLE = 0U,
    Motor_DISABLE,
} MotorFlag_t;

typedef enum
{
    Motor_Clockwise = 0U,
    Motor_Anticlockwise,
    Motor_Function,
    Motor_Shutoff
} Motor_SetState;

typedef enum
{
    Motor_Location = 0U,
    Motor_Location_line,
    Motor_Location_Angle,
} MotorStatus_t;

typedef struct
{
    int16_t Period;
    MotorFlag_t Flag;
} MotorFeedback_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} MotorAngle_t;

typedef struct
{
    int32_t Count;
    int32_t Actual_Target;  //��ǰλ��
    int32_t Actual_Speed;   //ʵ���ٶ�
    int32_t TargetValue;    //Ŀ��λ��/��ǰ�ٶ�
    Motor_SetState State;   //״̬
    MotorFlag_t Flag;       //�����־
} MotorControl_t;

typedef struct
{
    int8_t LineOffset;
    int8_t LineAngle;
    MotorAngle_t Angle;         //Ŀ��Ƕ�
    MotorAngle_t Actual_Angle;  //��ǰ�Ƕ�
    MotorStatus_t Status;       //״̬
    _Bool Flag;       //��־
} MotorCore_t;

typedef MotorFeedback_t MotorEncoder_t;

typedef void (*TTS_MotorSpeed_t)(uint8_t ID, uint16_t Speed);

typedef void  (*TTS_MotorStart_t)(uint8_t ID);

typedef void (*TTS_MotorStop_t)(uint8_t ID);

/** Control **/
void TTS_Motor_Config(MotorControl_t *User_Motor, uint16_t Speed, uint8_t Sum);

void TTS_Motor_BspControlConfig(TTS_MotorStart_t Start, TTS_MotorStop_t Stop, TTS_MotorSpeed_t Speed);

void TTS_MotorMove(uint8_t ID, uint16_t Speed);
/** Feedback **/
void TTS_MotorFeedback_Config(MotorFeedback_t *User_Feedback);

#endif/*_TTS_MOTOR_H*/
