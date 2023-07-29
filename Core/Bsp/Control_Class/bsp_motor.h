#ifndef _BSP_MOTOR_H
#define _BSP_MOTOR_H

#include "main.h"
#include "../User/User.h"

#define MLeft_TIM htim1
#define MRight_TIM htim1
#define MRightEncoder_TIM htim3
#define MLeftEncoder_TIM htim4
#define TARGET_PWM_MAX    400
#define TARGET_SPEED_MAX    120    // Ŀ���ٶȵ����ֵ r/m
#define Default_SPEED 90
/* ���ٵ�����ٱ� */
#define REDUCTION_RATIO                        21.3
/* ����������ֱ��� */
#define ENCODER_RESOLUTION                     11
/* 4��Ƶ����ֱܷ��� */
#define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)

/*��� 180 �Ƕȼ���*/
#define STEERING_GEAR_ANGLE(angle) (0.5 + (angle) / 180.0 * (2.5 - 0.5)) / 20.0 * 2000;    // ����Ƕȶ�Ӧ��ռ�ձ�
/*��ͷ������� 2000�Ƕ�ʱ������*/

#define Headstock_ANGLE(angle) (uint16_t)((0.5 + (90-angle) / 180.0 * (2.5 - 0.5)) / 20.0 * __HAL_TIM_GET_AUTORELOAD(&MDir_TIM))    // ����Ƕȶ�Ӧ��ռ�ձ�

typedef enum
{
    MLeft = 0U,
    MRight,
    MSum
} Motor_TypeDef;

void Motor_Init(MotorControl_t *User_Motor);

void MotorStart(uint8_t ID);

void MotorStop(uint8_t ID);

void MotorSpeed(uint8_t ID, uint16_t Speed);

void MotorHandle(void);

void EncoderHandle(Motor_TypeDef Motor_N, _Bool Flag);

int32_t Motor_CurrentPosition(Motor_TypeDef Motor_N);
void Motor_EncoderClear(void);


#define Motor_GetSpeed_0()  (Motor[MLeft].Actual_Speed == 0 && Motor[MRight].Actual_Speed == 0)

#define Motor_AngleReversal() (MotorCore.Angle.Y = (MotorCore.Angle.Y == -90) ? 90 : -90)

#define Motor_AngleDifference() (abs((int) (MotorCore.Angle.Y * 10) - (int) (MotorCore.Actual_Angle.Y * 10)) <= 10)
#endif /* _BSP_MOTOR_H */
