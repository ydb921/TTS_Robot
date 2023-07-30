#include "bsp_hc_sr04.h"


static float First_order_filter(float Com_angle, float angle_pt, float angle, float dt)
{
    float A, K = 0.330; // 对加速度计取值的权重
    A = K / (K + dt);
    Com_angle = A * (Com_angle + angle_pt * dt) + (1 - A) * angle;
    return Com_angle;
}

_Bool HC_SR04Distance(HC_SR04_t *User, TIM_HandleTypeDef *htim,
                      uint32_t Channel, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    float Distance = 0;
    switch (User->Capture_Cnt) {
        case 0: HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
            Delay_us(50);
            HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
//		__HAL_TIM_SET_COUNTER(htim, 0);
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, Channel, TIM_INPUTCHANNELPOLARITY_RISING);
            HAL_TIM_IC_Start_IT(htim, Channel);
            User->Capture_Cnt++;
            break;
        case 3:
            if (User->ECho_Time_TTL_H > User->ECho_Time_TTL_L)
                Distance = ((User->ECho_Time_TTL_H - User->ECho_Time_TTL_L) / 1000000.0) * 340.0 / 2.0 * 100.0;
            else
                Distance =
                    (((User->ECho_Time_TTL_H + 65535) - User->ECho_Time_TTL_L) / 1000000.0) * 340.0 / 2.0 * 100.0;
            User->Distance = First_order_filter(Distance, 0.33, 1.7, 0.01);
            if (User->Distance > 350)
                User->Distance = 999;
            delay_us(10);
            User->Capture_Cnt = 0;  //清空标志位
            return 1;
    }
    return 0;
}