#include "../Bsp/MPU6050/mpu6050.h"
#include "../Bsp/MPU6050/inv_mpu.h"
float pitch,roll,yaw;         //欧拉角
uint16_t aacx,aacy,aacz;        //加速度传感器原始数据
uint16_t gyrox,gyroy,gyroz;    //陀螺仪原始数据
uint16_t temp;                    //温度
uint16_t mpu_count;

void Control_Proc(void)
{
    switch (MotorCore.Task) {
        case MotorTask_Stop:

            break;
        case MotorTask_Beeline:

            break;
        case MotorTaskCrossroad:break;
        case MotorTask_T_line:break;
        case MotorTask_T_Left:break;
        case MotorTask_T_Right:break;
        case MotorTask_Left:break;
        case MotorTask_Right:break;
    }
}