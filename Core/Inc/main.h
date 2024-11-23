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
#include "stm32f1xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define stepmotor0_dir_Pin GPIO_PIN_13
#define stepmotor0_dir_GPIO_Port GPIOC
#define stepmotor0_pulse_Pin GPIO_PIN_14
#define stepmotor0_pulse_GPIO_Port GPIOC
#define stepmotor2_dir_Pin GPIO_PIN_15
#define stepmotor2_dir_GPIO_Port GPIOC
#define HongWai0_Pin GPIO_PIN_0
#define HongWai0_GPIO_Port GPIOA
#define stepmotor2_pulse_Pin GPIO_PIN_1
#define stepmotor2_pulse_GPIO_Port GPIOA
#define fan2_Pin GPIO_PIN_0
#define fan2_GPIO_Port GPIOB
#define mpu6050_SCL_Pin GPIO_PIN_10
#define mpu6050_SCL_GPIO_Port GPIOB
#define mpu6050_SDA_Pin GPIO_PIN_11
#define mpu6050_SDA_GPIO_Port GPIOB
#define sw3_Pin GPIO_PIN_12
#define sw3_GPIO_Port GPIOB
#define sw2_Pin GPIO_PIN_13
#define sw2_GPIO_Port GPIOB
#define sw1_Pin GPIO_PIN_14
#define sw1_GPIO_Port GPIOB
#define sw0_Pin GPIO_PIN_15
#define sw0_GPIO_Port GPIOB
#define sr04_echo_TIM1_CH1_Pin GPIO_PIN_8
#define sr04_echo_TIM1_CH1_GPIO_Port GPIOA
#define sr04_trig_Pin GPIO_PIN_9
#define sr04_trig_GPIO_Port GPIOA
#define dip2_Pin GPIO_PIN_10
#define dip2_GPIO_Port GPIOA
#define dip1_Pin GPIO_PIN_11
#define dip1_GPIO_Port GPIOA
#define dip0_Pin GPIO_PIN_12
#define dip0_GPIO_Port GPIOA
#define oled_SCL_Pin GPIO_PIN_8
#define oled_SCL_GPIO_Port GPIOB
#define oled_SDA_Pin GPIO_PIN_9
#define oled_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
