//
// Created by ashkore on 2024/4/13.
//

#include "beep.h"
#include "math.h"
Beep beep;

void beep_init(Beep *beep_struct, TIM_HandleTypeDef *tim, uint32_t channel)
{
    beep_struct->tim = tim;
    beep_struct->channel = channel;
    HAL_TIM_PWM_Start(beep_struct->tim, beep_struct->channel);
}

void beep_set_frequency(Beep *beep_struct, int frequency)
{
    uint32_t new_psc = (uint32_t)sqrt(SystemCoreClock / frequency);
    uint32_t new_arr = new_psc;
    __HAL_TIM_SET_AUTORELOAD(beep_struct->tim, new_arr);
    __HAL_TIM_SET_PRESCALER(beep_struct->tim, new_psc);

    if(frequency > 0){
        __HAL_TIM_SET_COMPARE(beep_struct->tim, beep_struct->channel, new_arr / 8);
    } else {
        __HAL_TIM_SET_COMPARE(beep_struct->tim, beep_struct->channel, 0);
    }
}
