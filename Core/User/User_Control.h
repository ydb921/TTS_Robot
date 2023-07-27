#include "../Bsp/MPU6050/mpu6050.h"
#include "../Bsp/MPU6050/inv_mpu.h"

float pitch, roll, yaw;         //欧拉角
uint16_t aacx, aacy, aacz;        //加速度传感器原始数据
uint16_t gyrox, gyroy, gyroz;    //陀螺仪原始数据
uint16_t temp;                    //温度
uint16_t mpu_count;

void Control_Proc(void)
{
    if (MotorCore.Flag)
        switch (MotorCore.Task.Set) {
            case MotorTask_Stop:
//    MotorCore.Status = Motor_Location_Angle;
                MotorCore.Status = Motor_Location;
                break;
            case MotorTask_Beeline:

                break;
            case MotorTaskCrossroad: {
                if (MotorCore.Task.Line_Status == Motor_LineNLL) {
                    MotorCore.Status = Motor_Location;
                }
                else {
                    MotorCore.Status = Motor_Location_line;
                    MotorCore.Task.Line_Status = MotorCrossroad;
                    TTS_MotorSetMove(210, 210);
                }
                break;
            }
            case MotorTask_T_line:break;
            case MotorTask_T_Left:break;
            case MotorTask_T_Right:break;
            case MotorTask_Left:break;
            case MotorTask_Right:break;
        }
}