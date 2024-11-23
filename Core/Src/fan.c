//
// Created by ashkore on 2024/4/12.
//

#include "fan.h"

Fan fan;

Fan_gpio fan_gpio;

void fan_init(Fan *fan_struct, TIM_HandleTypeDef *tim, uint32_t channelA, uint32_t channelB)
{
    fan_struct->tim = tim;
    fan_struct->channelA = channelA;
    fan_struct->channelB = channelB;
    HAL_TIM_PWM_Start(fan_struct->tim, fan_struct->channelA);
    HAL_TIM_PWM_Start(fan_struct->tim, fan_struct->channelB);
}

void fan_gpio_init(Fan_gpio *fan_gpio_struct, TIM_HandleTypeDef *tim, uint32_t channelA, GPIO_TypeDef *portB, uint32_t pinB)
{
    fan_gpio_struct->tim = tim;
    fan_gpio_struct->channelA = channelA;
    fan_gpio_struct->portB = portB;
    fan_gpio_struct->pinB = pinB;
    HAL_TIM_PWM_Start(fan_gpio_struct->tim, fan_gpio_struct->channelA);
}

// speed: -100 to 100
void fan_set_speed(Fan *fan_struct, float speed)
{
    uint32_t tim_arr = __HAL_TIM_GET_AUTORELOAD(fan_struct->tim);

    if(speed > FAN_MAX_DUTY){
        speed = FAN_MAX_DUTY;
    } else if(speed < -FAN_MAX_DUTY){
        speed = -FAN_MAX_DUTY;
    }

    if(speed > 0){
        __HAL_TIM_SET_COMPARE(fan_struct->tim, fan_struct->channelA, speed * tim_arr / 100);
        __HAL_TIM_SET_COMPARE(fan_struct->tim, fan_struct->channelB, 0);
    } else if(speed < 0){
        __HAL_TIM_SET_COMPARE(fan_struct->tim, fan_struct->channelA, 0);
        __HAL_TIM_SET_COMPARE(fan_struct->tim, fan_struct->channelB, -speed * tim_arr / 100);
    } else {
        __HAL_TIM_SET_COMPARE(fan_struct->tim, fan_struct->channelA, 0);
        __HAL_TIM_SET_COMPARE(fan_struct->tim, fan_struct->channelB, 0);
    }
}

// speed: -100 to 100
void fan_gpio_set_speed(Fan_gpio *fan_gpio_struct, float speed)
{
    uint32_t tim_arr = __HAL_TIM_GET_AUTORELOAD(fan_gpio_struct->tim);

    if(speed > FAN_MAX_DUTY){
        speed = FAN_MAX_DUTY;
    } else if(speed < -FAN_MAX_DUTY){
        speed = -FAN_MAX_DUTY;
    }

    if(speed > 0){
        __HAL_TIM_SET_COMPARE(fan_gpio_struct->tim, fan_gpio_struct->channelA, speed * tim_arr / 100);
        HAL_GPIO_WritePin(fan_gpio_struct->portB, fan_gpio_struct->pinB, GPIO_PIN_RESET);
    } else if(speed < 0){
        HAL_GPIO_WritePin(fan_gpio_struct->portB, fan_gpio_struct->pinB, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(fan_gpio_struct->tim, fan_gpio_struct->channelA, -speed * tim_arr / 100);
    } else {
        __HAL_TIM_SET_COMPARE(fan_gpio_struct->tim, fan_gpio_struct->channelA, 0);
        HAL_GPIO_WritePin(fan_gpio_struct->portB, fan_gpio_struct->pinB, GPIO_PIN_RESET);
    }
}