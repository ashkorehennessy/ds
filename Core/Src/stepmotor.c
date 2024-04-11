//
// Created by ashkore on 2024/4/9.
//

#include "stepmotor.h"
#include "delay_us.h"
#include "math.h"

StepMotor_gpio_t stepmotor_gpio;

void StepMotor_gpio_init(StepMotor_gpio_t *stepmotor_struct, GPIO_TypeDef *dir_port, uint16_t dir_pin, GPIO_TypeDef *pulse_port, uint16_t pulse_pin){
    stepmotor_struct->dir_port = dir_port;
    stepmotor_struct->dir_pin = dir_pin;
    stepmotor_struct->pulse_port = pulse_port;
    stepmotor_struct->pulse_pin = pulse_pin;
    HAL_GPIO_WritePin(stepmotor_struct->dir_port, stepmotor_struct->dir_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(stepmotor_struct->pulse_port, stepmotor_struct->pulse_pin, GPIO_PIN_RESET);
}

void StepMotor_gpio_run(StepMotor_gpio_t *stepmotor_struct, uint32_t steps, int speed){
    uint32_t delay;
    if(speed < 0) {
        HAL_GPIO_WritePin(stepmotor_struct->dir_port, stepmotor_struct->dir_pin, GPIO_PIN_SET);
        delay = -(1000000 / speed);
    } else {
        HAL_GPIO_WritePin(stepmotor_struct->dir_port, stepmotor_struct->dir_pin, GPIO_PIN_RESET);
        delay = 1000000 / speed;
    }
    for(int i = 0; i < steps; i++){
        HAL_GPIO_WritePin(stepmotor_struct->pulse_port, stepmotor_struct->pulse_pin, GPIO_PIN_SET);
        Delay_us(delay);
        HAL_GPIO_WritePin(stepmotor_struct->pulse_port, stepmotor_struct->pulse_pin, GPIO_PIN_RESET);
        Delay_us(delay);
    }
}

StepMotor_pwm_t stepmotor_pwm;

void StepMotor_pwm_init(StepMotor_pwm_t *stepmotor_struct, GPIO_TypeDef *dir_port, uint16_t dir_pin, TIM_HandleTypeDef *pulse_tim, uint32_t pulse_channel){
    stepmotor_struct->dir_port = dir_port;
    stepmotor_struct->dir_pin = dir_pin;
    stepmotor_struct->pulse_tim = pulse_tim;
    stepmotor_struct->pulse_channel = pulse_channel;
    HAL_GPIO_WritePin(stepmotor_struct->dir_port, stepmotor_struct->dir_pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Start(stepmotor_struct->pulse_tim, stepmotor_struct->pulse_channel);
    __HAL_TIM_SET_COMPARE(stepmotor_struct->pulse_tim, stepmotor_struct->pulse_channel, __HAL_TIM_GET_AUTORELOAD(stepmotor_struct->pulse_tim) / 2);
}

void StepMotor_pwm_change_frequency(StepMotor_pwm_t *stepmotor_struct, int frequency){
    if(frequency > 0){
        HAL_GPIO_WritePin(stepmotor_struct->dir_port, stepmotor_struct->dir_pin, GPIO_PIN_SET);
    } else if(frequency < 0){
        HAL_GPIO_WritePin(stepmotor_struct->dir_port, stepmotor_struct->dir_pin, GPIO_PIN_RESET);
        frequency = -frequency;
    } else {
        __HAL_TIM_SET_COMPARE(stepmotor_struct->pulse_tim, stepmotor_struct->pulse_channel, 0);
        return;
    }
    uint32_t new_psc = (uint32_t)sqrt(SystemCoreClock / frequency);
    uint32_t new_arr = new_psc;
    __HAL_TIM_SET_PRESCALER(stepmotor_struct->pulse_tim, new_psc - 1);
    __HAL_TIM_SET_AUTORELOAD(stepmotor_struct->pulse_tim, new_arr);
    __HAL_TIM_SET_COMPARE(stepmotor_struct->pulse_tim, stepmotor_struct->pulse_channel, new_arr / 2);
}