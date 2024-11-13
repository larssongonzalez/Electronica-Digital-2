/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define segc_Pin GPIO_PIN_13
#define segc_GPIO_Port GPIOB
#define segb_Pin GPIO_PIN_14
#define segb_GPIO_Port GPIOB
#define sega_Pin GPIO_PIN_15
#define sega_GPIO_Port GPIOB
#define led_rojo_Pin GPIO_PIN_6
#define led_rojo_GPIO_Port GPIOC
#define led_verde_Pin GPIO_PIN_8
#define led_verde_GPIO_Port GPIOC
#define segg_Pin GPIO_PIN_9
#define segg_GPIO_Port GPIOC
#define segf_Pin GPIO_PIN_8
#define segf_GPIO_Port GPIOA
#define sege_Pin GPIO_PIN_9
#define sege_GPIO_Port GPIOA
#define segd_Pin GPIO_PIN_10
#define segd_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
