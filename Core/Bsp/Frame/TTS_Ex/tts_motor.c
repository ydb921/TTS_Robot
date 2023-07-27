#include "tts_motor.h"

static MotorControl_t *Motor;

static TTS_MotorSpeed_t MotorSpeed;

static TTS_MotorStart_t MotorStart;

static TTS_MotorStop_t MotorStop;
void TTS_MotorMove(uint8_t ID, uint16_t Speed)
{
    switch (Motor[ID].State) {
        case Motor_Function:MotorStart(ID);
            break;
        case Motor_Shutoff:MotorSpeed(ID, 0);
            MotorStop(ID);
            break;
        default:
            MotorStart(ID);
            MotorSpeed(ID, Speed);
            break;
    }
}
void TTS_Motor_Config(MotorControl_t *User_Motor, uint16_t TargetValue, uint8_t Sum)
{
    Motor = User_Motor;
    for (uint8_t i = 0; i < Sum; ++i) {
        MotorStop(i);
        Motor[i].State = Motor_Shutoff;
        Motor[i].Flag = Motor_DISABLE;
        Motor[i].TargetValue = TargetValue;
        Motor[i].Actual_Speed = 0;
        Motor[i].Actual_Target = 0;
        Motor[i].Count = 0;
    }
}

void TTS_Motor_BspControlConfig(TTS_MotorStart_t Start, TTS_MotorStop_t Stop, TTS_MotorSpeed_t Speed)
{
    MotorSpeed = Speed;
    MotorStart = Start;
    MotorStop = Stop;
}

void TTS_MotorFeedback_Config(MotorFeedback_t *User_Feedback)
{
    User_Feedback->Period = 0;

    User_Feedback->Flag = Motor_DISABLE;
}
