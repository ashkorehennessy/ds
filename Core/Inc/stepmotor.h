//
// Created by ashkore on 2024/4/9.
//

#ifndef DS_STEPMOTOR_H
#define DS_STEPMOTOR_H

#include "stm32f1xx_hal.h"

typedef struct{
    GPIO_TypeDef *dir_port;
    uint16_t dir_pin;
    GPIO_TypeDef *pulse_port;
    uint16_t pulse_pin;
} StepMotor_gpio_t;

typedef struct {
    GPIO_TypeDef *dir_port;
    uint16_t dir_pin;
    TIM_HandleTypeDef *pulse_tim;
    uint32_t pulse_channel;
} StepMotor_pwm_t;

extern StepMotor_gpio_t stepmotor_gpio;

extern StepMotor_pwm_t stepmotor_pwm;

void StepMotor_gpio_init(StepMotor_gpio_t *stepmotor_struct, GPIO_TypeDef *dir_port, uint16_t dir_pin, GPIO_TypeDef *pulse_port, uint16_t pulse_pin);

void StepMotor_gpio_run(StepMotor_gpio_t *stepmotor_struct, uint32_t steps, int speed);

void StepMotor_pwm_init(StepMotor_pwm_t *stepmotor_struct, GPIO_TypeDef *dir_port, uint16_t dir_pin, TIM_HandleTypeDef *pulse_tim, uint32_t pulse_channel);

void StepMotor_pwm_change_frequency(StepMotor_pwm_t *stepmotor_struct, int frequency);



#endif //DS_STEPMOTOR_H
