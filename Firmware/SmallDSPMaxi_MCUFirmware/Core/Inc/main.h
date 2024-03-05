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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <Codecs.h>
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
#define DSP_GPIO6_Pin GPIO_PIN_3
#define DSP_GPIO6_GPIO_Port GPIOA
#define DSP_GPIO7_Pin GPIO_PIN_4
#define DSP_GPIO7_GPIO_Port GPIOA
#define DSP_GPIO14_Pin GPIO_PIN_5
#define DSP_GPIO14_GPIO_Port GPIOA
#define DSP_GPIO15_Pin GPIO_PIN_6
#define DSP_GPIO15_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_7
#define LED3_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOB
#define NRST_DSP_Pin GPIO_PIN_10
#define NRST_DSP_GPIO_Port GPIOB
#define NCS_Codec5_Pin GPIO_PIN_12
#define NCS_Codec5_GPIO_Port GPIOB
#define NCS_Codec6_Pin GPIO_PIN_13
#define NCS_Codec6_GPIO_Port GPIOB
#define NCS_Codec7_Pin GPIO_PIN_14
#define NCS_Codec7_GPIO_Port GPIOB
#define NCS_Codec8_Pin GPIO_PIN_15
#define NCS_Codec8_GPIO_Port GPIOB
#define NCS_Codec1_Pin GPIO_PIN_8
#define NCS_Codec1_GPIO_Port GPIOA
#define NCS_Codec2_Pin GPIO_PIN_9
#define NCS_Codec2_GPIO_Port GPIOA
#define NCS_Codec3_Pin GPIO_PIN_10
#define NCS_Codec3_GPIO_Port GPIOA
#define NCS_Codec4_Pin GPIO_PIN_11
#define NCS_Codec4_GPIO_Port GPIOA
#define ErrorFlag_Pin GPIO_PIN_12
#define ErrorFlag_GPIO_Port GPIOA
#define EN_Codecs_1to4_Pin GPIO_PIN_6
#define EN_Codecs_1to4_GPIO_Port GPIOF
#define EN_Codecs_5to8_Pin GPIO_PIN_7
#define EN_Codecs_5to8_GPIO_Port GPIOF

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
