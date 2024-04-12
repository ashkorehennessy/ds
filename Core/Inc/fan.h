//
// Created by ashkore on 2024/4/12.
//

#ifndef DS_FAN_H
#define DS_FAN_H

#define FAN_MAX_DUTY 65

#include "stm32f1xx_hal.h"

typedef struct{
    TIM_HandleTypeDef *tim;
    uint32_t channelA;
    uint32_t channelB;
} Fan;

extern Fan fan;

void fan_init(Fan *fan_struct, TIM_HandleTypeDef *tim, uint32_t channelA, uint32_t channelB);

void fan_set_speed(Fan *fan_struct, float speed);

#endif //DS_FAN_H
