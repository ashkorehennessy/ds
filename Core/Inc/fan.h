//
// Created by ashkore on 2024/4/12.
//

#ifndef DS_FAN_H
#define DS_FAN_H

#define FAN_MAX_DUTY 1000

#include "stm32f1xx_hal.h"

typedef struct{
    TIM_HandleTypeDef *tim;
    uint32_t channelA;
    uint32_t channelB;
} Fan;

typedef struct{
    TIM_HandleTypeDef *tim;
    uint32_t channelA;
    GPIO_TypeDef *portB;
    uint32_t pinB;
} Fan_gpio;

extern Fan fan;

extern Fan_gpio fan_gpio;

void fan_init(Fan *fan_struct, TIM_HandleTypeDef *tim, uint32_t channelA, uint32_t channelB);

void fan_gpio_init(Fan_gpio *fan_gpio_struct, TIM_HandleTypeDef *tim, uint32_t channelA, GPIO_TypeDef *portB, uint32_t pinB);

void fan_set_speed(Fan *fan_struct, float speed);

void fan_gpio_set_speed(Fan_gpio *fan_gpio_struct, float speed);

#endif //DS_FAN_H
