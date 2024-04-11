//
// Created by ashkore on 2024/4/11.
//

#ifndef DS_SMALLSTEPMOTOR_H
#define DS_SMALLSTEPMOTOR_H

#include "stm32f1xx_hal.h"

typedef struct {
    GPIO_TypeDef *portA;
    uint16_t pinA;
    GPIO_TypeDef *portB;
    uint16_t pinB;
    GPIO_TypeDef *portC;
    uint16_t pinC;
    GPIO_TypeDef *portD;
    uint16_t pinD;
} smallstepmotor_gpio_t;

typedef struct {
    TIM_HandleTypeDef *tim;
    uint32_t channelA;
    uint32_t channelB;
    uint32_t channelC;
    uint32_t channelD;
} smallstepmotor_pwm_t;

extern smallstepmotor_gpio_t smallstepmotor_gpio;
extern smallstepmotor_pwm_t smallstepmotor_pwm;

void smallstepmotor_gpio_init(smallstepmotor_gpio_t *smallstepmotor_struct, GPIO_TypeDef *portA, uint16_t pinA, GPIO_TypeDef *portB, uint16_t pinB, GPIO_TypeDef *portC, uint16_t pinC, GPIO_TypeDef *portD, uint16_t pinD);

void smallstepmotor_gpio_run(smallstepmotor_gpio_t *smallstepmotor_struct, uint32_t times, int frequency);

void smallstepmotor_pwm_init(smallstepmotor_pwm_t *smallstepmotor_struct, TIM_HandleTypeDef *tim, uint32_t channelA, uint32_t channelB, uint32_t channelC, uint32_t channelD);

void smallstepmotor_pwm_change_frequency(smallstepmotor_pwm_t *smallstepmotor_struct, int frequency);
#endif //DS_SMALLSTEPMOTOR_H
