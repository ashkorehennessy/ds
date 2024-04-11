//
// Created by ashkore on 2024/1/24.
//

#ifndef SMART_CAR_CAMERA_SWITCH_H
#define SMART_CAR_CAMERA_SWITCH_H

#include "stm32f1xx_hal.h"

#define DIP_SWITCH_1 (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12))
#define DIP_SWITCH_2 (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11))
#define DIP_SWITCH_3 (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10))
#define DIP_SWITCH   ((DIP_SWITCH_1) | (DIP_SWITCH_2) << 1 | (DIP_SWITCH_3) << 2)

#define SWITCH_1 (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15))
#define SWITCH_2 (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
#define SWITCH_3 (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13))
#define SWITCH_4 (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))

int dip_switch();

int switch1();

int switch2();

int switch3();

int switch4();

#endif //SMART_CAR_CAMERA_SWITCH_H
