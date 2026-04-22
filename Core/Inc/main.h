/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "AD9959.h"
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
#define SDIO2_Pin GPIO_PIN_11
#define SDIO2_GPIO_Port GPIOB
#define SDIO1_Pin GPIO_PIN_12
#define SDIO1_GPIO_Port GPIOB
#define SDIO3_Pin GPIO_PIN_13
#define SDIO3_GPIO_Port GPIOB
#define SDIO0_Pin GPIO_PIN_14
#define SDIO0_GPIO_Port GPIOB
#define SCLK_Pin GPIO_PIN_15
#define SCLK_GPIO_Port GPIOB
#define PS3_Pin GPIO_PIN_8
#define PS3_GPIO_Port GPIOD
#define CS_Pin GPIO_PIN_9
#define CS_GPIO_Port GPIOD
#define PS2_Pin GPIO_PIN_10
#define PS2_GPIO_Port GPIOD
#define UPDATE_Pin GPIO_PIN_11
#define UPDATE_GPIO_Port GPIOD
#define PS1_Pin GPIO_PIN_12
#define PS1_GPIO_Port GPIOD
#define RESET_Pin GPIO_PIN_13
#define RESET_GPIO_Port GPIOD
#define PS0_Pin GPIO_PIN_14
#define PS0_GPIO_Port GPIOD
#define PDC_Pin GPIO_PIN_15
#define PDC_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
