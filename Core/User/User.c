#include "User.h"
/*點擊控制*/
#include "User_Motor.h"
/*協議解析*/
#include "User_Spcp.h"
/*顯示代碼*/
#include "User_display.h"
/*輸入控制*/
#include "User_key.h"
/*APP控制*/
#include "User_Control.h"

/* 定时器任务句柄 */
static Stu_TimerTypeDef OS_Timer[OS_Timer_SUM];

/* 时间句柄 */
Time_TypeDef CurrentTime;

void UserProc(void)
{
    char arr_AT[30];
    if (MotorCore.Flag && mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
        MotorCore.Actual_Angle.X = (pitch);
        MotorCore.Actual_Angle.Y = (yaw);
        MotorCore.Actual_Angle.Z = (roll);
        MotorCore.Actual_Angle.Y = MotorCore.Actual_Angle.Y - 0.001 * mpu_count - 0.49;
//                temp = MPU_Get_Temperature();                                //得到温度值
//                MPU_Get_Accelerometer(&aacx, &aacy, &aacz);    //得到加速度传感器数据
//                MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);    //得到陀螺仪数据
//        sprintf(arr_AT, "{A%d:%d:%d:%d}$", MotorCore.LineOffset, MotorCore.LineAngle, 0, (int16_t) yaw);
        sprintf(arr_AT,
                "{A%d:%d:%d:%d}$",
                MotorCore.Line.Offset, MotorCore.Line.Angle,
                MotorCore.Line.Status, (int16_t) (MotorCore.Actual_Angle.Y));
        TTSQueue_SendByte_(ATTask_Debug, arr_AT);
//                printf("三轴角度：%.2f  %.2f  %.2f\r\n", pitch, roll, yaw);
//        printf("三轴加速度：%d-%d-%d\r\n",aacx,aacy,aacz);
//        printf("三轴角角度：%d-%d-%d\r\n",gyrox,gyroy,gyroz);
//        temp = MPU_Get_Temperature();                                //得到温度值
//        MPU_Get_Accelerometer(&aacx, &aacy, &aacz);    //得到加速度传感器数据
//        MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);    //得到陀螺仪数据
//        sprintf(arr_AT, "{A%d:%d:%d:%d}$", MotorCore.LineOffset, MotorCore.LineAngle, 0, (int16_t) yaw);
//        TTSQueue_SendByte_(ATTask_Debug, arr_AT);
//                printf("三轴角度：%.2f  %.2f  %.2f\r\n", pitch, roll, yaw);
//        printf("三轴加速度：%d-%d-%d\r\n",aacx,aacy,aacz);
//        printf("三轴角角度：%d-%d-%d\r\n",gyrox,gyroy,gyroz);
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
static void PinProc(void)
{
    TTS_PinProc();
    ultrasonicProc();
}
void User_Create_task(void)
{
    /** Task Creation **/
    if (TTS_TaskCreation(OS_TASK_Pin, PinProc, TTS_PinProcTick, OS_RUN) != OS_RUN)
        Error_Handler();
    if (TTS_TaskCreation(OS_TASK_Button, TTS_ButtonProc, TTS_ButtonProcTick, OS_RUN) != OS_RUN)
        Error_Handler();
    if (TTS_TaskCreation(OS_TASK_Proc, UserProc, 10, OS_RUN) != OS_RUN)
        Error_Handler();
    if (TTS_TaskCreation(OS_TASK_Queue, Queue_Proc, 5, OS_Pause) != OS_Pause)
        Error_Handler();
    if (TTS_TaskCreation(OS_TASK_Control, Control_Proc, 300, OS_RUN) != OS_RUN)
        Error_Handler();
    if (TTS_TimerCreation(OS_Timer_Time, Time_Proc, 100, TTS_Timer_START) != TTS_Timer_START)
        Error_Handler();
    if (TTS_TimerCreation(OS_MPU_Clock, MPU_Proc, 100, TTS_Timer_START) != TTS_Timer_START)
        Error_Handler();
    if (TTS_TimerCreation(OS_Motor_Clock, MotorHandle, 10, TTS_Timer_START) != TTS_Timer_START)
        Error_Handler();
    TTS_SetStatus(OS_TASK_Queue, OS_RUN);
//    MotorCore.Task.Set = MotorTaskCrossroad;
//    MotorCoreSetStatus(Motor_Location_line);
//    TTS_MotorSetMove(Default_SPEED, Default_SPEED);
}

void User_Bsp_Init(void)
{
    HAL_Delay(1);//等待系统稳定运行
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
    while (MPU_Init())//初始化MPU6050
        Delay_ms(1);
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

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim9.Instance == htim->Instance) {
        if (HAL_TIM_ACTIVE_CHANNEL_2 == htim->Channel)
            HC_SR04Time(USER_HC_SR04[HC_SR04_Right], *htim, TIM_CHANNEL_2);
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
            HC_SR04Time(USER_HC_SR04[HC_SR04_left], *htim, TIM_CHANNEL_1);
    }
}
