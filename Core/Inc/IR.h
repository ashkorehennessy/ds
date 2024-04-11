//
// Created by ashkore on 2024/4/8.
//

#ifndef DS_IR_H
#define DS_IR_H

#include "stm32f1xx_hal.h"

typedef struct{
    uint32_t count;
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t start_time;
    uint32_t frequency;
    int state;
} IR_t;

extern IR_t ir;

void IR_init(IR_t *IR_struct, GPIO_TypeDef *port, uint16_t pin);

void IR_exti_callback(IR_t *IR_struct);

int IR_get_state(IR_t *IR_struct);

uint32_t IR_get_count(IR_t *IR_struct);

uint32_t IR_get_frequency(IR_t *IR_struct);

#endif //DS_IR_H
