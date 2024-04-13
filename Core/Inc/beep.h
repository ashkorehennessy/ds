//
// Created by ashkore on 2024/4/13.
//

#ifndef DS_BEEP_H
#define DS_BEEP_H

#include "stm32f1xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *tim;
    uint32_t channel;
} Beep;

extern Beep beep;

void beep_init(Beep *beep_struct, TIM_HandleTypeDef *tim, uint32_t channel);

void beep_set_frequency(Beep *beep_struct, int frequency);

#endif //DS_BEEP_H
