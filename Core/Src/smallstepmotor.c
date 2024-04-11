//
// Created by ashkore on 2024/4/11.
//

#include <math.h>
#include "smallstepmotor.h"
#include "delay_us.h"

smallstepmotor_gpio_t smallstepmotor_gpio;

smallstepmotor_pwm_t smallstepmotor_pwm;

void smallstepmotor_gpio_init(smallstepmotor_gpio_t *smallstepmotor_struct, GPIO_TypeDef *portA, uint16_t pinA, GPIO_TypeDef *portB, uint16_t pinB, GPIO_TypeDef *portC, uint16_t pinC, GPIO_TypeDef *portD, uint16_t pinD) {
    smallstepmotor_struct->portA = portA;
    smallstepmotor_struct->pinA = pinA;
    smallstepmotor_struct->portB = portB;
    smallstepmotor_struct->pinB = pinB;
    smallstepmotor_struct->portC = portC;
    smallstepmotor_struct->pinC = pinC;
    smallstepmotor_struct->portD = portD;
    smallstepmotor_struct->pinD = pinD;
    HAL_GPIO_WritePin(smallstepmotor_struct->portA, smallstepmotor_struct->pinA, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(smallstepmotor_struct->portB, smallstepmotor_struct->pinB, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(smallstepmotor_struct->portC, smallstepmotor_struct->pinC, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(smallstepmotor_struct->portD, smallstepmotor_struct->pinD, GPIO_PIN_RESET);
}

void smallstepmotor_gpio_run(smallstepmotor_gpio_t *smallstepmotor_struct, uint32_t ms, int frequency) {
    uint32_t delay;
    HAL_GPIO_WritePin(smallstepmotor_struct->portA, smallstepmotor_struct->pinA, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(smallstepmotor_struct->portB, smallstepmotor_struct->pinB, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(smallstepmotor_struct->portC, smallstepmotor_struct->pinC, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(smallstepmotor_struct->portD, smallstepmotor_struct->pinD, GPIO_PIN_RESET);
    if (frequency < 0) {
        delay = -(1000000 / frequency) / 4;
        for(uint32_t start = HAL_GetTick(); HAL_GetTick() - start < ms;){
            HAL_GPIO_WritePin(smallstepmotor_struct->portD, smallstepmotor_struct->pinD, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(smallstepmotor_struct->portA, smallstepmotor_struct->pinA, GPIO_PIN_SET);
            Delay_us(delay);
            HAL_GPIO_WritePin(smallstepmotor_struct->portA, smallstepmotor_struct->pinA, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(smallstepmotor_struct->portB, smallstepmotor_struct->pinB, GPIO_PIN_SET);
            Delay_us(delay);
            HAL_GPIO_WritePin(smallstepmotor_struct->portB, smallstepmotor_struct->pinB, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(smallstepmotor_struct->portC, smallstepmotor_struct->pinC, GPIO_PIN_SET);
            Delay_us(delay);
            HAL_GPIO_WritePin(smallstepmotor_struct->portC, smallstepmotor_struct->pinC, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(smallstepmotor_struct->portD, smallstepmotor_struct->pinD, GPIO_PIN_SET);
            Delay_us(delay);
        }
    } else if (frequency > 0) {
        delay = (1000000 / frequency) / 4;
        for(uint32_t start = HAL_GetTick(); HAL_GetTick() - start < ms;){
            HAL_GPIO_WritePin(smallstepmotor_struct->portA, smallstepmotor_struct->pinA, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(smallstepmotor_struct->portD, smallstepmotor_struct->pinD, GPIO_PIN_SET);
            Delay_us(delay);
            HAL_GPIO_WritePin(smallstepmotor_struct->portD, smallstepmotor_struct->pinD, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(smallstepmotor_struct->portC, smallstepmotor_struct->pinC, GPIO_PIN_SET);
            Delay_us(delay);
            HAL_GPIO_WritePin(smallstepmotor_struct->portC, smallstepmotor_struct->pinC, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(smallstepmotor_struct->portB, smallstepmotor_struct->pinB, GPIO_PIN_SET);
            Delay_us(delay);
            HAL_GPIO_WritePin(smallstepmotor_struct->portB, smallstepmotor_struct->pinB, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(smallstepmotor_struct->portA, smallstepmotor_struct->pinA, GPIO_PIN_SET);
            Delay_us(delay);
        }
    }
}

void smallstepmotor_pwm_init(smallstepmotor_pwm_t *smallstepmotor_struct, TIM_HandleTypeDef *tim, uint32_t channelA, uint32_t channelB, uint32_t channelC, uint32_t channelD) {
    smallstepmotor_struct->tim = tim;
    smallstepmotor_struct->channelA = channelA;
    smallstepmotor_struct->channelB = channelB;
    smallstepmotor_struct->channelC = channelC;
    smallstepmotor_struct->channelD = channelD;
    HAL_TIM_PWM_Start(smallstepmotor_struct->tim, smallstepmotor_struct->channelA);
    HAL_TIM_PWM_Start(smallstepmotor_struct->tim, smallstepmotor_struct->channelB);
    HAL_TIM_PWM_Start(smallstepmotor_struct->tim, smallstepmotor_struct->channelC);
    HAL_TIM_PWM_Start(smallstepmotor_struct->tim, smallstepmotor_struct->channelD);
    __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelA, __HAL_TIM_GET_AUTORELOAD(smallstepmotor_struct->tim) / 4);
    __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelB, __HAL_TIM_GET_AUTORELOAD(smallstepmotor_struct->tim) / 4);
    __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelC, __HAL_TIM_GET_AUTORELOAD(smallstepmotor_struct->tim) / 4);
    __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelD, __HAL_TIM_GET_AUTORELOAD(smallstepmotor_struct->tim) / 4);
}

void smallstepmotor_pwm_change_frequency(smallstepmotor_pwm_t *smallstepmotor_struct, int frequency) {
    if (frequency > 0) {
        frequency = -frequency;
    } else if (frequency < 0) {
        frequency = -frequency;
    } else {
        __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelA, 0);
        __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelB, 0);
        __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelC, 0);
        __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelD, 0);
        return;
    }
    uint32_t new_psc = (uint32_t) sqrt(SystemCoreClock / frequency);
    uint32_t new_arr = new_psc;
    __HAL_TIM_SET_PRESCALER(smallstepmotor_struct->tim, new_psc - 1);
    __HAL_TIM_SET_AUTORELOAD(smallstepmotor_struct->tim, new_arr);
    __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelA, new_arr / 4);
    __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelB, new_arr / 4);
    __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelC, new_arr / 4);
    __HAL_TIM_SET_COMPARE(smallstepmotor_struct->tim, smallstepmotor_struct->channelD, new_arr / 4);
}


