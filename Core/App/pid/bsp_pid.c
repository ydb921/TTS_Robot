#include "bsp_pid.h"

void pid_init(_pid *pid, float Kp, float Ki, float Kd)
{
    pid->target_val = 0;
    pid->actual_val = 0.0;
    pid->err = 0.0;
    pid->err_last = 0.0;
    pid->integral = 0.0;
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}

/**
  * @brief  PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  * @retval ͨ��PID���������
  */
float pid_realize(_pid *pid, float actual_val, void(*function)(_pid *pid))
{
    /*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err = pid->target_val - actual_val;
    function(pid);
    // ����ۻ�
    pid->integral += pid->err;
    /*PID�㷨ʵ��*/
    pid->actual_val = pid->Kp * pid->err +
        pid->Ki * pid->integral +
        pid->Kd * (pid->err - pid->err_last);
    /*����*/
    pid->err_last = pid->err;
    /*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}

/**
 * �趨�ٶȱջ�����
 * @param pid
 */
void pid_location_ead(_pid *pid)
{

    if ((pid->err >= -15) && (pid->err <= 15)) {
        pid->err = 0;
        pid->integral = 0;
    }
}

/**
 * �趨�ٶȱջ�����
 * @param pid
 */
void pid_speed_ead(_pid *pid)
{
//	if ((pid->err < 0.2f) && (pid->err > -0.2f))
//		pid->err = 0.0f;
    if (pid->err < 0.3 && pid->err > -0.3) pid->err = 0;//pid����
    pid->integral += pid->err;
}

/**
 * �趨�ٶȱջ�����
 * @param pid
 */
void pid_NULL(_pid *pid)
{

}

