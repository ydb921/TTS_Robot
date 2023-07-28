#include "User.h"
/*�c������*/
#include "User_Motor.h"
/*�f�h����*/
#include "User_Spcp.h"
/*�@ʾ���a*/
#include "User_display.h"
/*ݔ�����*/
#include "User_key.h"
/*APP����*/
#include "User_Control.h"

/* ��ʱ�������� */
static Stu_TimerTypeDef OS_Timer[OS_Timer_SUM];

/* ʱ���� */
Time_TypeDef CurrentTime;

void UserProc(void)
{
    char arr_AT[30];
    if (MotorCore.Flag && mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
        MotorCore.Actual_Angle.X = (pitch);
        MotorCore.Actual_Angle.Y = (yaw);
        MotorCore.Actual_Angle.Z = (roll);
        MotorCore.Actual_Angle.Y = MotorCore.Actual_Angle.Y - 0.001 * mpu_count - 0.49;
//                temp = MPU_Get_Temperature();                                //�õ��¶�ֵ
//                MPU_Get_Accelerometer(&aacx, &aacy, &aacz);    //�õ����ٶȴ���������
//                MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);    //�õ�����������
//        sprintf(arr_AT, "{A%d:%d:%d:%d}$", MotorCore.LineOffset, MotorCore.LineAngle, 0, (int16_t) yaw);
        sprintf(arr_AT,
                "{A%d:%d:%d:%d}$",
                MotorCore.Line.Offset, MotorCore.Line.Angle,
                MotorCore.Line.Status, (int16_t) (MotorCore.Actual_Angle.Y * 10));
        TTSQueue_SendByte_(ATTask_Debug, arr_AT);
//                printf("����Ƕȣ�%.2f  %.2f  %.2f\r\n", pitch, roll, yaw);
//        printf("������ٶȣ�%d-%d-%d\r\n",aacx,aacy,aacz);
//        printf("����ǽǶȣ�%d-%d-%d\r\n",gyrox,gyroy,gyroz);
//        temp = MPU_Get_Temperature();                                //�õ��¶�ֵ
//        MPU_Get_Accelerometer(&aacx, &aacy, &aacz);    //�õ����ٶȴ���������
//        MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);    //�õ�����������
//        sprintf(arr_AT, "{A%d:%d:%d:%d}$", MotorCore.LineOffset, MotorCore.LineAngle, 0, (int16_t) yaw);

//        TTSQueue_SendByte_(ATTask_Debug, arr_AT);
//                printf("����Ƕȣ�%.2f  %.2f  %.2f\r\n", pitch, roll, yaw);
//        printf("������ٶȣ�%d-%d-%d\r\n",aacx,aacy,aacz);
//        printf("����ǽǶȣ�%d-%d-%d\r\n",gyrox,gyroy,gyroz);
    }
    else if (MotorCore.Flag == DISABLE && mpu_dmp_init() == 0)
        MotorCore.Flag = ENABLE;
}

static void Time_Proc(void)
{
    TTS_CurrentTimeHandle(&CurrentTime, 100);
}

static void MPU_Proc(void)
{
    if (++mpu_count == 6000)mpu_count = 0;
}

void User_Create_task(void)
{
    /** Task Creation **/
    if (TTS_TaskCreation(OS_TASK_Pin, TTS_PinProc, TTS_PinProcTick, OS_RUN) != OS_RUN)
        Error_Handler();
    if (TTS_TaskCreation(OS_TASK_Button, TTS_ButtonProc, TTS_ButtonProcTick, OS_RUN) != OS_RUN)
        Error_Handler();
    if (TTS_TaskCreation(OS_TASK_Proc, UserProc, 10, OS_RUN) != OS_RUN)
        Error_Handler();
    if (TTS_TaskCreation(OS_TASK_Queue, Queue_Proc, 5, OS_Pause) != OS_Pause)
        Error_Handler();
    if (TTS_TaskCreation(OS_TASK_Control, Control_Proc, 200, OS_RUN) != OS_RUN)
        Error_Handler();
    if (TTS_TimerCreation(OS_Timer_Time, Time_Proc, 100, TTS_Timer_START) != TTS_Timer_START)
        Error_Handler();
    if (TTS_TimerCreation(OS_MPU_Clock, MPU_Proc, 100, TTS_Timer_START) != TTS_Timer_START)
        Error_Handler();
    if (TTS_TimerCreation(OS_Motor_Clock, MotorHandle, 10, TTS_Timer_START) != TTS_Timer_START)
        Error_Handler();
    TTS_SetStatus(OS_TASK_Queue, OS_RUN);
}

void User_Bsp_Init(void)
{
    HAL_Delay(10);//�ȴ�ϵͳ�ȶ�����
    /* Task Init */
    TTS_TaskInit(OS_TASK_SUM);
    /* Buzzer&Led Init */
    TTS_PinInit();
    /* Button Init */
    TTS_ButtonInit(Button_EventCallBack);
    /* Queue Configuration */
    UART_Config();
    /* Spcp Init */
    SPCPConfig();
    /* Motor Init */
    Motor_Init(Motor);
    /* Time Init */
    TTS_TimerInit(OS_Timer, OS_Timer_SUM);
    HAL_TIM_Base_Start_IT(&User_Timer_Tim);
    /* MPU Init */
    while (MPU_Init())//��ʼ��MPU6050
        Delay_ms(10);
    MotorCore.Task.Set = MotorTaskCrossroad;
    MotorCore.Task.Line_Status = MotorCrossroad;

//    MotorCore.Task.Line_Status= Motor_LineStop;

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &User_Timer_Tim)
        TTS_TimerHandle();
    else if (htim == (&MLeftEncoder_TIM))
        EncoderHandle(MLeft, __HAL_TIM_IS_TIM_COUNTING_DOWN(&MLeftEncoder_TIM));
    else if (htim == (&MRightEncoder_TIM))
        EncoderHandle(MRight, __HAL_TIM_IS_TIM_COUNTING_DOWN(&MRightEncoder_TIM));
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
//    if (htim == &MRight_TIM)
//        MotorRPeriod();
//    else if (htim == &MLeft_TIM)
//        MotorLPeriod();
}
