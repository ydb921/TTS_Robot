#include "../Bsp/MPU6050/mpu6050.h"
#include "../Bsp/MPU6050/inv_mpu.h"
float pitch,roll,yaw;         //ŷ����
uint16_t aacx,aacy,aacz;        //���ٶȴ�����ԭʼ����
uint16_t gyrox,gyroy,gyroz;    //������ԭʼ����
uint16_t temp;                    //�¶�
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