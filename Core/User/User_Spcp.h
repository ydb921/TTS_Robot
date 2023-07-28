#include <stdlib.h>
#include "../Bsp/Communication_Class/bsp_usart.h"

extern Queue_TypeDef USART_Queue[QueueTask_Num];

static uSPCP_t UserSPCP[QueueTask_Num];

void Spcp_Callback(uint8_t ID, uSPCPData_t *Data, uint8_t Length, const uint8_t *FrameData)
{
    if (QueueTask_Debug == ID) {
        printf("id: %d ,Data: %s ,Length: %d FrameData: %x \n", ID, Data, Length, *FrameData);
    }
    else if (LineTask_Debug == ID) {
        static _Bool Line_Flag = DISABLE;
        MotorCore.Line.Status = (MotorLine_t) Data[0];
        MotorCore.Line.Offset = (int8_t) Data[1];
        MotorCore.Line.Angle = (int8_t) Data[2];
        if (MotorCore.Status != Motor_Location_line)
            return;
        if (MotorCore.Line.Status == MotorCore.Task.Line_Status) {
            if (MotorCore.Line.Status == Motor_LineStop) {
                Line_Flag = DISABLE;
                MotorCore.Task.Line_Status = Motor_LineNLL;
                TTS_MotorSetStop();
            }
            else {
                Line_Flag = ENABLE;
            }
        }
        else if (Line_Flag == ENABLE) {
            Line_Flag = DISABLE;
            MotorCore.Task.Line_Status = Motor_LineNLL;
            TTS_MotorSetStop();
        }
    }
    else if (ATTask_Debug == ID) {
        if (Length == 1) {
            if (*Data == 'Z') {
                TTS_MotorSetStop();
//            Motor[MLeft].TargetValue=Motor_CurrentPosition(MLeft);
//            Motor[MRight].TargetValue=Motor_CurrentPosition(MRight);
            }
            else if (*Data == 'A') {
                MotorCore.Angle.Y = 0;
                Motor[MLeft].TargetValue += 45;
                Motor[MRight].TargetValue += 45;
            }
            else if (*Data == 'E') {
                MotorCore.Angle.Y = 0;
                Motor[MLeft].TargetValue -= 45;
                Motor[MRight].TargetValue -= 45;
            }
            else if (*Data == 'G') {
                switch (MotorCore.Status) {

                    case Motor_Location:
                    case Motor_Location_line:Motor[MLeft].TargetValue += 45;
                        Motor[MRight].TargetValue -= 45;
                        break;
                    case Motor_Location_Angle:MotorCore.Angle.Y = 90;
                        break;
                }

            }
            else if (*Data == 'C') {
                switch (MotorCore.Status) {
                    case Motor_Location:
                    case Motor_Location_line:Motor[MLeft].TargetValue -= 45;
                        Motor[MRight].TargetValue += 45;
                        break;
                    case Motor_Location_Angle:MotorCore.Angle.Y = -90;
                        break;
                }

            }

            else if (*Data == 'm') {
                MotorCore.Angle.Y = -90;
            }
            else if (*Data == 'o') {
                MotorCore.Angle.Y = 0;
            }
            else if (*Data == 'n') {
                MotorCore.Angle.Y = 90;
            }
        }
    }
//    else if (ATTask_Debug == ID) {
//
//    }
    else {
        for (int i = 0; i < Length; i++)
            printf(" %x ", Data[i]);
        printf("\n");
    }
}

void SPCPConfig(void)
{
    Queue_Task_Enum_TypeDef SizeQueue_Task;
    Spcp_SetCallback((TTSSPCP_Callback_t) Spcp_Callback);
    uint8_t Sum = 0;
    SizeQueue_Task = QueueTask_Debug;
    Spcp_Creation(&UserSPCP[SizeQueue_Task], 5);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Frame, '1', DISABLE);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Frame, '5', DISABLE);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Length_ASCII, 1, DISABLE);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Data, 1, ENABLE);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Frame, '2', DISABLE);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Check, XOR_Check, DISABLE);
    Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Callback, SizeQueue_Task, DISABLE);
    /* 7e 02 10 7C 1C 10 FE */
    Sum = 0;
    SizeQueue_Task = LineTask_Debug;
    Spcp_Creation(&UserSPCP[SizeQueue_Task], 6);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Frame, 0x7e, DISABLE);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Length_Hex, 0x00, DISABLE);
//    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_FrameX, 0x01, DISABLE);
//    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Frame, 0x7C, DISABLE);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Data, 1, DISABLE);
    Sum = Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Frame, 0xFE, DISABLE);
    Spcp_SetData(&UserSPCP[SizeQueue_Task], Sum, Status_Callback, SizeQueue_Task, DISABLE);
}

void Queue_Proc(void)
{
    TTS_SpcpProc(&UserSPCP[QueueTask_Debug], &USART_Queue[QueueTask_Debug].Rx_Data);
//
//    Type_Queue_Data data;
//    if (QueueDataLen(USART_Queue[LineTask_Debug].Rx_Data)) {
//        TTSQueueDataOutByte(USART_Queue[LineTask_Debug].Rx_Data, &data);
//        TTSQueue_Send(LineTask_Debug, &data, 1);
//        if (data == '1') {
//            TTS_Task_WritePeriod(OS_TASK_Pin, 1000);
//        }
//        else if (data == '2') {
//            TTS_Task_WritePeriod(OS_TASK_Pin, 500);
//        }
//        else if (data == '3') {
//            TTS_Task_WritePeriod(OS_TASK_Pin, 50);
//        }
//    }
//    else
//        TTS_SetStatus(OS_TASK_Queue, OS_Pause);
}