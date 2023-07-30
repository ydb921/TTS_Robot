#ifndef _BSP_HC_SR04_H
#define _BSP_HC_SR04_H

#include "main.h"
#include "../User/User.h"

#define HC_SR04Time(User, htim, TIM_CHANNEL) \
    do{                                                                     \
        switch (User.Capture_Cnt)                                                                 \
        {                                                                                 \
            case 1:                                                      \
                User.ECho_Time_TTL_L = HAL_TIM_ReadCapturedValue(&htim, TIM_CHANNEL); \
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim, TIM_CHANNEL, TIM_ICPOLARITY_FALLING); \
                User.Capture_Cnt++;                               \
                break;                                                      \
            case 2:                                                      \
                User.ECho_Time_TTL_H = HAL_TIM_ReadCapturedValue(&htim, TIM_CHANNEL);                         \
                HAL_TIM_IC_Stop_IT(&htim, TIM_CHANNEL);                                       \
                User.Capture_Cnt++;                                         \
                break;                                                         \
        }                                                             \
    }while(0)

enum
{
    HC_SR04_left,
    HC_SR04_Right,
    HC_SR04_Sum,
};

typedef struct
{
    uint8_t Capture_Cnt;//×´Ì¬±êÖ¾Î»
    uint32_t ECho_Time_TTL_L;
    uint32_t ECho_Time_TTL_H;
    float Distance;   //¾àÀë
    void (*draw_btn)(void *btn);
} HC_SR04_t;

_Bool
HC_SR04Distance(HC_SR04_t *User, TIM_HandleTypeDef *htim, uint32_t Channel, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /* _BSP_HC_SR04_H */
