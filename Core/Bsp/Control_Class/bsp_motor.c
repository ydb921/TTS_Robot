#include "bsp_motor.h"
#include "../../App/pid/bsp_pid.h"
#include "../Bsp/Communication_Class/bsp_usart.h"

extern MotorControl_t Motor[MSum];

extern MotorCore_t MotorCore;

static MotorEncoder_t Motor_Encoder[MSum];

static int32_t MotorOffset[MSum];

static _pid pid_location[MSum];

static _pid pid_speed[MSum];

static _pid pid_Angle;

static _pid pid_Line;

static _pid pid_Line_Theta;

void Motor_Init(MotorControl_t *User_Motor)
{
    /** TTS Motor Control Init **/
    TTS_Motor_BspControlConfig(MotorStart, MotorStop, MotorSpeed);
    TTS_Motor_Config(User_Motor, 0, MSum);
    /** Encoder **/
    TTS_MotorFeedback_Config(&Motor_Encoder[MLeft]);
    TTS_MotorFeedback_Config(&Motor_Encoder[MRight]);
    __HAL_TIM_CLEAR_IT(&MLeftEncoder_TIM, TIM_IT_UPDATE);//δ�ҵ���Ӧ����
    if (HAL_TIM_Base_Start_IT(&MLeftEncoder_TIM) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_Encoder_Start(&MLeftEncoder_TIM, TIM_CHANNEL_ALL) != HAL_OK)
        Error_Handler();

    /*Right �����жϱ�־λ */
    __HAL_TIM_CLEAR_IT(&MRightEncoder_TIM, TIM_IT_UPDATE);//δ�ҵ���Ӧ����
    if (HAL_TIM_Base_Start_IT(&MRightEncoder_TIM) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_Encoder_Start(&MRightEncoder_TIM, TIM_CHANNEL_ALL) != HAL_OK)
        Error_Handler();

    HAL_GPIO_WritePin(MotorAll_Stop_GPIO_Port, MotorAll_Stop_Pin, GPIO_PIN_SET);
    MotorCore.Flag = DISABLE;
//    MotorSpeed(MLeft, 100, 0);
//    MotorSpeed(MRight, 100, 0);
//    Motor[MLeft].State = Motor_Clockwise;
//    Motor[MRight].State = Motor_Clockwise;
    /* PID ������ʼ�� */
    pid_init(&pid_speed[MLeft], (float) 2, (float) 0.4, (float) 3);
    pid_init(&pid_speed[MRight], (float) 2, (float) 0.4, (float) 3);
    pid_init(&pid_location[MLeft], (float) 0.158, (float) 0.0002, (float) 0);
    pid_init(&pid_location[MRight], (float) 0.158, (float) 0.0002, (float) 0);
    pid_init(&pid_Angle, (float) 0.0025, (float) 0.00, (float) 0.04);
    pid_init(&pid_Line, (float) 0.3, (float) 0.00, (float) 1.5);
    pid_init(&pid_Line_Theta, (float) 0.01, (float) 0.00, (float) 0.5);
    while (0) {

    }
}

float Motor_SpeedControl(MotorControl_t *UserMotor, Motor_TypeDef Motor_N)
{
    /** ��һʱ���ܼ���ֵ */
    static __IO int32_t Last_Count[MSum] = {0, 0};

//    if (UserMotor[Motor_N].State != Motor_Shutoff) {
    /** ת��ת�� = ��λʱ���ڵļ���ֵ / �������ֱܷ��� * ʱ��ϵ��  */
    UserMotor[Motor_N].Actual_Speed = (int32_t) (((float) (Motor[Motor_N].Count - Last_Count[Motor_N]) /
        ENCODER_TOTAL_RESOLUTION / REDUCTION_RATIO) / (10 / 1000.0 / 60.0));

    /** ��¼��ǰ�ܼ���ֵ������һʱ�̼���ʹ�� */
    Last_Count[Motor_N] = Motor[Motor_N].Count;
    /**  PID ����  */
    float cont_val = pid_realize(&pid_speed[Motor_N], (float) UserMotor[Motor_N].Actual_Speed, pid_speed_ead);
    /** �жϵ������ */
    if (cont_val > 0) {
        Motor[Motor_N].State = Motor_Clockwise;
    }
    else {
        cont_val = -cont_val;
        Motor[Motor_N].State = Motor_Anticlockwise;
    }
    /** �ٶ����޴��� **/
    cont_val = (cont_val > TARGET_PWM_MAX) ? TARGET_PWM_MAX : cont_val;
    return cont_val;
//    }
    return 0;
}

float Motor_LocationControl(MotorControl_t *UserMotor, Motor_TypeDef Motor_N)
{
    float cont_val = pid_realize(&pid_location[Motor_N],
                                 (float) UserMotor[Motor_N].Count, pid_location_ead);
    /* Ŀ���ٶ����޴��� */
    if (cont_val > TARGET_SPEED_MAX) {
        cont_val = TARGET_SPEED_MAX;
    }
    else if (cont_val < -TARGET_SPEED_MAX) {
        cont_val = -TARGET_SPEED_MAX;
    }
    return cont_val;

}

void MotorHandle(void)
{
    static uint8_t location_timer;
    static uint8_t Angle_timer;
    uint16_t Motor_Speed[MSum];
    /** ��ǰʱ���ܼ���ֵ = ������ֵ + ����������� * ENCODER_TIM_PERIOD  */
    Motor[MLeft].Count = __HAL_TIM_GET_COUNTER(&MLeftEncoder_TIM) + (Motor_Encoder[MLeft].Period * 65535);
    Motor[MRight].Count = __HAL_TIM_GET_COUNTER(&MRightEncoder_TIM) + (Motor_Encoder[MRight].Period * 65535);
    if (location_timer++ % 2 == 0) {
        int16_t MotorGetOffset;
        if (Angle_timer++ % 2 == 0)
            switch (MotorCore.Status) {
                case Motor_Location: {
                    MotorOffset[MLeft] = 0;
                    MotorOffset[MRight] = 0;
                    break;
                }
                case Motor_Location_line: {
                    if (MotorCore.Line.Status == Motor_Beeline) {
                        MotorGetOffset = pid_realize(&pid_Line, (float) (MotorCore.Line.Offset), pid_NULL) +
                            pid_realize(&pid_Line_Theta, (float) (MotorCore.Line.Angle), pid_NULL);
//                        if (MotorGetOffset > 6) {
//                            MotorGetOffset = 6;
//                        }
//                        else if (MotorGetOffset < -6) {
//                            MotorGetOffset = -6;
//                        }
                        MotorOffset[MLeft] -= MotorGetOffset;
                        MotorOffset[MRight] += MotorGetOffset;
                    }
                    break;
                }
                case Motor_Location_Angle: {
                    MotorGetOffset = pid_realize(&pid_Angle, (float) (MotorCore.Angle.Y - MotorCore.Actual_Angle.Y),
                                                 pid_NULL);
//                MotorGetOffset = (MotorCore.Actual_Angle.Y - MotorCore.Angle.Y);
                    MotorOffset[MLeft] += MotorGetOffset;
                    MotorOffset[MRight] -= MotorGetOffset;
                    break;
                }
                default:Error_Handler();
                    break;
            }
        /** ����pidĿ��ֵ */
        pid_location[MLeft].target_val =
            (float) (((Motor[MLeft].TargetValue + MotorOffset[MLeft]) * ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO)
                / 210.0);
        /** ����pidĿ��ֵ */
        pid_location[MRight].target_val =
            (float) (((Motor[MRight].TargetValue + MotorOffset[MRight]) * ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO)
                / 210.0);
        /** ����pidĿ��ֵ */
        pid_speed[MLeft].target_val = Motor_LocationControl(Motor, MLeft);
        /** ����pidĿ��ֵ */
        pid_speed[MRight].target_val = Motor_LocationControl(Motor, MRight);
    }
    //    /** ����pidĿ��ֵ */
//    pid_speed[MLeft].target_val = (float) Motor[MLeft].TargetValue;
//    /** ����pidĿ��ֵ */
//    pid_speed[MRight].target_val = (float) Motor[MRight].TargetValue;
    Motor_Speed[MLeft] = (uint16_t) Motor_SpeedControl(Motor, MLeft);
    Motor_Speed[MRight] = (uint16_t) Motor_SpeedControl(Motor, MRight);
    TTS_MotorMove(MLeft, Motor_Speed[MLeft]);
    TTS_MotorMove(MRight, Motor_Speed[MRight]);
//    char CBuff[50];
//    sprintf(CBuff, "{B%d:%d:}$", (int16_t) MotorCore.Angle.Y, (int16_t) MotorCore.Actual_Angle.Y);
//    sprintf(CBuff, "{B%d:%d:}$", (int16_t) Motor[MRight].Actual_Speed, (int16_t) pid_speed[MRight].target_val);
//    sprintf(CBuff,
//            "{B%d:%d:}$",
//            (int16_t) ((Motor[MRight].TargetValue * ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO) / 210.0),
//            (int16_t) Motor[MRight].Count);
//    TTSQueue_SendByte_(ATTask_Debug, CBuff);
}

void EncoderHandle(Motor_TypeDef Motor_N, _Bool Flag)
{
    if (Flag)
        Motor_Encoder[Motor_N].Period--;
    else
        Motor_Encoder[Motor_N].Period++;
}

void TTS_MotorSetMove(int32_t L, int32_t R)
{
    Motor[MLeft].TargetValue += L;
    Motor[MRight].TargetValue += R;
}

void TTS_MotorSetStop(void)
{
    Motor[MLeft].TargetValue =
        (int32_t) ((Motor[MLeft].TargetValue / ENCODER_TOTAL_RESOLUTION / REDUCTION_RATIO) * 210.0) + 105;
    Motor[MRight].TargetValue =
        (int32_t) ((Motor[MRight].TargetValue / ENCODER_TOTAL_RESOLUTION / REDUCTION_RATIO) * 210.0) + 105;
}

void TTS_MotorSetZero(void)
{
    if (Motor[MLeft].TargetValue
        == (int32_t) (((Motor[MLeft].TargetValue / ENCODER_TOTAL_RESOLUTION / REDUCTION_RATIO) * 210.0)) &&
        Motor[MRight].TargetValue
            == (int32_t) (((Motor[MRight].TargetValue / ENCODER_TOTAL_RESOLUTION / REDUCTION_RATIO) * 210.0))
        ) {
        Motor[MLeft].TargetValue = 0;
        Motor[MRight].TargetValue = 0;
        MotorOffset[MLeft] = 0;
        Motor_Encoder[MLeft].Period = 0;
        Motor[MLeft].Actual_Speed = 0;
        Motor[MLeft].Count = 0;
        __HAL_TIM_SET_COUNTER(&MLeftEncoder_TIM, 0);
        MotorOffset[MRight] = 0;
        Motor_Encoder[MRight].Period = 0;
        Motor[MRight].Actual_Speed = 0;
        Motor[MRight].Count = 0;
        __HAL_TIM_SET_COUNTER(&MRightEncoder_TIM, 0);
    }
}

void MotorSpeed(uint8_t ID, uint16_t Speed)
{
    switch (ID) {
        case MLeft: __HAL_TIM_SetCompare (&MLeft_TIM, TIM_CHANNEL_1, Speed);
            if (Motor[MLeft].State) {
                HAL_GPIO_WritePin(MotorL_DirA_GPIO_Port, MotorL_DirA_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MotorL_DirB_GPIO_Port, MotorL_DirB_Pin, GPIO_PIN_RESET);
            }
            else {
                HAL_GPIO_WritePin(MotorL_DirA_GPIO_Port, MotorL_DirA_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MotorL_DirB_GPIO_Port, MotorL_DirB_Pin, GPIO_PIN_SET);
            }
            break;
        case MRight:__HAL_TIM_SetCompare (&MRight_TIM, TIM_CHANNEL_2, Speed);
            if (Motor[MRight].State) {
                HAL_GPIO_WritePin(MotorR_DirA_GPIO_Port, MotorR_DirA_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MotorR_DirB_GPIO_Port, MotorR_DirB_Pin, GPIO_PIN_RESET);
            }
            else {
                HAL_GPIO_WritePin(MotorR_DirA_GPIO_Port, MotorR_DirA_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MotorR_DirB_GPIO_Port, MotorR_DirB_Pin, GPIO_PIN_SET);
            }
            break;
        default:Error_Handler();
            break;
    }
}

void MotorStart(uint8_t ID)
{
    switch (ID) {
        case MLeft:HAL_TIM_PWM_Start(&MLeft_TIM, TIM_CHANNEL_1);
            break;
        case MRight:HAL_TIM_PWM_Start(&MRight_TIM, TIM_CHANNEL_2);
            break;
        default:Error_Handler();
            break;
    }
}

void MotorStop(uint8_t ID)
{
    switch (ID) {
        case MLeft:HAL_TIM_PWM_Stop(&MLeft_TIM, TIM_CHANNEL_1);
            HAL_GPIO_WritePin(MotorL_DirA_GPIO_Port, MotorL_DirA_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MotorL_DirB_GPIO_Port, MotorL_DirB_Pin, GPIO_PIN_RESET);
            break;
        case MRight:HAL_TIM_PWM_Stop(&MRight_TIM, TIM_CHANNEL_2);
            HAL_GPIO_WritePin(MotorR_DirA_GPIO_Port, MotorR_DirA_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MotorR_DirB_GPIO_Port, MotorR_DirB_Pin, GPIO_PIN_RESET);
            break;
        default:Error_Handler();
            break;
    }
}
