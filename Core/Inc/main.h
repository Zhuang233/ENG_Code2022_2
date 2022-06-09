/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define APB1_TIMER_CLOCKS 84000000
#define TIM_PSC_APB1 ((APB1_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1)
#define PWM_FREQUENCE 50
#define PWM_RESOLUTION 10000
#define APB2_TIMER_CLOCKS 168000000
#define TIM_PSC_APB2 ((APB2_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1)
#define SWITCH_STRETCH_Pin GPIO_PIN_5
#define SWITCH_STRETCH_GPIO_Port GPIOE
#define CAMERA_Pin GPIO_PIN_6
#define CAMERA_GPIO_Port GPIOE
#define POWER1_Pin GPIO_PIN_2
#define POWER1_GPIO_Port GPIOH
#define POWER2_Pin GPIO_PIN_3
#define POWER2_GPIO_Port GPIOH
#define SWITCH_FLIP_Pin GPIO_PIN_1
#define SWITCH_FLIP_GPIO_Port GPIOF
#define POWER3_Pin GPIO_PIN_4
#define POWER3_GPIO_Port GPIOH
#define POWER4_Pin GPIO_PIN_5
#define POWER4_GPIO_Port GPIOH
#define VALVE11_Pin GPIO_PIN_2
#define VALVE11_GPIO_Port GPIOC
#define VALVE12_Pin GPIO_PIN_3
#define VALVE12_GPIO_Port GPIOC
#define KEY_Pin GPIO_PIN_2
#define KEY_GPIO_Port GPIOB
#define VALVE2_Pin GPIO_PIN_4
#define VALVE2_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_11
#define LED_RED_GPIO_Port GPIOE
#define LED_GREEN_Pin GPIO_PIN_14
#define LED_GREEN_GPIO_Port GPIOF
#define SENSOR_Pin GPIO_PIN_12
#define SENSOR_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#define KEY HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)  //user key PB2
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
