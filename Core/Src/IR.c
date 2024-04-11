//
// Created by ashkore on 2024/4/8.
//

#include "IR.h"

IR_t ir;

void IR_init(IR_t *IR_struct, GPIO_TypeDef *port, uint16_t pin){
    IR_struct->count = 0;
    IR_struct->port = port;
    IR_struct->pin = pin;
    IR_struct->start_time = 0;
    HAL_GPIO_WritePin(IR_struct->port, IR_struct->pin, GPIO_PIN_RESET);
}

void IR_exti_callback(IR_t *IR_struct){
    if(HAL_GPIO_ReadPin(IR_struct->port, IR_struct->pin) == GPIO_PIN_SET){
        IR_struct->count++;
    }
}

int IR_get_state(IR_t *IR_struct){
    IR_struct->state = HAL_GPIO_ReadPin(IR_struct->port, IR_struct->pin);
    return IR_struct->state;
}

uint32_t IR_get_count(IR_t *IR_struct){
    return IR_struct->count;
}

uint32_t IR_get_frequency(IR_t *IR_struct){
    uint32_t frequency = IR_struct->count / (HAL_GetTick() - IR_struct->start_time) * 1000;
    IR_struct->start_time = HAL_GetTick();
    IR_struct->count = 0;
    IR_struct->state = HAL_GPIO_ReadPin(IR_struct->port, IR_struct->pin);
    IR_struct->frequency = frequency;
    return frequency;
}
