#include "../Bsp/MPU6050/mpu6050.h"
#include "../Bsp/MPU6050/inv_mpu.h"

float pitch, roll, yaw;         //欧拉角
uint16_t aacx, aacy, aacz;        //加速度传感器原始数据
uint16_t gyrox, gyroy, gyroz;    //陀螺仪原始数据
uint16_t temp;                    //温度
uint16_t mpu_count;

void Control_Proc(void)
{
    static uint8_t TIme = 0;

    if (MotorCore.Flag)
        switch (MotorCore.Task.Set) {
            case MotorTask_Stop:
//    MotorCore.Status = Motor_Location_Angle;
//                MotorCore.Status = Motor_Location;
//                if (++TIme < 3) {

//                }
//                else if (TIme == 3) {
//                    MotorCore.Task.Set = MotorTaskCrossroad;
//                    MotorCore.Task.Line_Status = Motor_T_line;
//                }
//                if (MotorCoreSetStatus(Motor_Location_Angle)) {
//                    MotorCore.Task.Line_Status = Motor_LineNLL;
//                    MotorCore.Task.Set = MotorTask_T_line;
                if (MotorCore.Angle.Y != -90)
                    TTS_MotorSetMove(25, 25);

                    MotorCore.Angle.Y = -90;
//                }
                break;
            case MotorTask_Beeline:break;
            case MotorTaskCrossroad: {
                if (MotorCore.Task.Line_Status == Motor_LineNLL) {
                    if (MotorCoreSetStatus(Motor_Location_Angle)) {
                        MotorCore.Task.Set = MotorTask_Stop;
                    }
                }
                else if (Motor_Location_line != MotorCore.Status) {
                    if (MotorCoreSetStatus(Motor_Location_line)) {
                        TTS_MotorSetMove(80, 80);
                    }
                }
                break;
            }
            case MotorTask_T_line: {

                break;
            }
            case MotorTask_T_Left:break;
            case MotorTask_T_Right:break;
            case MotorTask_Left:break;
            case MotorTask_Right:break;
        }
}