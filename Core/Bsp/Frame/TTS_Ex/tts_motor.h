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
    Motor_Location_line,//Ѳ��
    Motor_Location_Angle,//�����ƶ�
} MotorStatus_t;

typedef enum
{
    Motor_LineStop = 0U,
    Motor_Beeline,  //ֱ��
    MotorCrossroad, //ʮ��·��
    Motor_T_line,        //ǰ��T��·��
    Motor_T_Left,        //���T��·��
    Motor_T_Right,        //�ұ�T��·��
    Motor_Left,     //��ת
    Motor_Right ,    //��ת
    Motor_LineNLL
} MotorLine_t;

typedef enum
{
    MotorTask_Stop = 0U,
    MotorTask_Beeline,  //ֱ��
    MotorTaskCrossroad, //ʮ��·��
    MotorTask_T_line,        //ǰ��T��·��
    MotorTask_T_Left,        //���T��·��
    MotorTask_T_Right,        //�ұ�T��·��
    MotorTask_Left,     //��ת
    MotorTask_Right     //��ת
} MotorSetTask_t;

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
    MotorLine_t Status;
    int8_t Offset;
    int8_t Angle;
} LineWalking_t;

typedef struct
{
    MotorSetTask_t Set;
    MotorLine_t Line_Status;
} MotorTask_t;

typedef struct
{
    LineWalking_t Line;
    MotorAngle_t Angle;         //Ŀ��Ƕ�
    MotorAngle_t Actual_Angle;  //��ǰ�Ƕ�
    MotorStatus_t Status;       //״̬
    MotorTask_t Task;
    _Bool Flag;       //��־
} MotorCore_t;

typedef MotorFeedback_t MotorEncoder_t;

typedef void (*TTS_MotorSpeed_t)(uint8_t ID, uint16_t Speed);

typedef void  (*TTS_MotorStart_t)(uint8_t ID);

typedef void (*TTS_MotorStop_t)(uint8_t ID);

/** Control **/
void TTS_Motor_Config(MotorControl_t *User_Motor, uint16_t Speed, uint8_t Sum);

void TTS_Motor_BspControlConfig(TTS_MotorStart_t Start, TTS_MotorStop_t Stop, TTS_MotorSpeed_t Speed);
void TTS_MotorSetMove(int32_t L, int32_t R);
void TTS_MotorMove(uint8_t ID, uint16_t Speed);
/** Feedback **/
void TTS_MotorFeedback_Config(MotorFeedback_t *User_Feedback);
void TTS_MotorSetStop(void);
void TTS_MotorSetZero(void);
#endif/*_TTS_MOTOR_H*/
