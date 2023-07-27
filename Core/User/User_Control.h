#include "../Bsp/MPU6050/mpu6050.h"
#include "../Bsp/MPU6050/inv_mpu.h"
float pitch,roll,yaw;         //欧拉角
uint16_t aacx,aacy,aacz;        //加速度传感器原始数据
uint16_t gyrox,gyroy,gyroz;    //陀螺仪原始数据
uint16_t temp;                    //温度
uint16_t mpu_count;