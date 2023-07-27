/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Bsp/Frame/TTS/CPU.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Button_1_Pin GPIO_PIN_3
#define Button_1_GPIO_Port GPIOE
#define Button_0_Pin GPIO_PIN_4
#define Button_0_GPIO_Port GPIOE
#define Button_Up_Pin GPIO_PIN_0
#define Button_Up_GPIO_Port GPIOA
#define Led_D2_Pin GPIO_PIN_6
#define Led_D2_GPIO_Port GPIOA
#define Led_D3_Pin GPIO_PIN_7
#define Led_D3_GPIO_Port GPIOA
#define MotorL_DirA_Pin GPIO_PIN_4
#define MotorL_DirA_GPIO_Port GPIOC
#define MotorL_DirB_Pin GPIO_PIN_5
#define MotorL_DirB_GPIO_Port GPIOC
#define MotorR_DirA_Pin GPIO_PIN_7
#define MotorR_DirA_GPIO_Port GPIOE
#define MotorR_DirB_Pin GPIO_PIN_8
#define MotorR_DirB_GPIO_Port GPIOE
#define MotorAll_Stop_Pin GPIO_PIN_14
#define MotorAll_Stop_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
