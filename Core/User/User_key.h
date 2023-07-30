#include "../Bsp/Control_Class/bsp_button.h"
#include "../Bsp/Control_Class/bsp_hc_sr04.h"

static HC_SR04_t USER_HC_SR04[HC_SR04_Sum];

void Button_EventCallBack(Button_Type Buttons, Button_Event_TypeDef State)
{
    if (State == Button_Click_Release) {
        switch (Buttons) {
            case Button_Up: {
//                TTS_Task_WritePeriod(OS_TASK_Pin, 1000);
                break;
            }
            case Button_0: {
//                MotorCore.Angle.Y+=9000;
                Motor[MRight].TargetValue += 210;
                Motor[MLeft].TargetValue += 210;
//                TTS_Task_WritePeriod(OS_TASK_Pin, 500);
                break;
            }
            case Button_1: {
//                MotorCore.Angle.Y-=9000;
                Motor[MLeft].TargetValue -= 210;
                Motor[MRight].TargetValue -= 210;
//                TTS_Task_WritePeriod(OS_TASK_Pin, 50);
                break;
            }
        }
    }

}

void ultrasonicProc(void)
{
    static _Bool ultrasonicProcFLag;
    if (ultrasonicProcFLag) {
        HC_SR04Distance(&USER_HC_SR04[HC_SR04_left], &htim9, TIM_CHANNEL_1,
                        TRIG_L_GPIO_Port, TRIG_L_Pin);
        ultrasonicProcFLag = 0;
        char CBuff[50];
        sprintf(CBuff,
                "{B%d:%d:}$",
                (int16_t) USER_HC_SR04[HC_SR04_left].Distance,
                (int16_t) USER_HC_SR04[HC_SR04_Right].Distance);
        TTSQueue_SendByte_(ATTask_Debug, CBuff);
    }
    else {
        HC_SR04Distance(&USER_HC_SR04[HC_SR04_Right], &htim9, TIM_CHANNEL_2,
                        TRIG_R_GPIO_Port, TRIG_R_Pin);
        ultrasonicProcFLag = 1;

    }
}

