#include "../Bsp/MPU6050/mpu6050.h"
#include "../Bsp/MPU6050/inv_mpu.h"
float pitch,roll,yaw;         //ŷ����
uint16_t aacx,aacy,aacz;        //���ٶȴ�����ԭʼ����
uint16_t gyrox,gyroy,gyroz;    //������ԭʼ����
uint16_t temp;                    //�¶�
uint16_t mpu_count;