//
// Created by ashkore on 2024/4/12.
//

#include "fan.h"

Fan fan;

void fan_init(Fan *fan_struct, TIM_HandleTypeDef *tim, uint32_t channelA, uint32_t channelB)
{
    fan_struct->tim = tim;
    fan_struct->channelA = channelA;
    fan_struct->channelB = channelB;
    HAL_TIM_PWM_Start(fan_struct->tim, fan_struct->channelA);
    HAL_TIM_PWM_Start(fan_struct->tim, fan_struct->channelB);
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
