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
  * @brief  PID算法实现
  * @param  actual_val:实际值
  * @retval 通过PID计算后的输出
  */
float pid_realize(_pid *pid, float actual_val, void(*function)(_pid *pid))
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;
    function(pid);
    // 误差累积
    pid->integral += pid->err;
    /*PID算法实现*/
    pid->actual_val = pid->Kp * pid->err +
        pid->Ki * pid->integral +
        pid->Kd * (pid->err - pid->err_last);
    /*误差传递*/
    pid->err_last = pid->err;
    /*返回当前实际值*/
    return pid->actual_val;
}

/**
 * 设定速度闭环死区
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
 * 设定速度闭环死区
 * @param pid
 */
void pid_speed_ead(_pid *pid)
{
//	if ((pid->err < 0.2f) && (pid->err > -0.2f))
//		pid->err = 0.0f;
    if (pid->err < 0.3 && pid->err > -0.3) pid->err = 0;//pid死区
    pid->integral += pid->err;
}

/**
 * 设定速度闭环死区
 * @param pid
 */
void pid_NULL(_pid *pid)
{

}

