//
// Created by ashkore on 2024/4/9.
//

#include "delay_us.h"

void Delay_us(uint32_t us){
    uint32_t delay = us * (SystemCoreClock / 10000000);
    for(uint32_t i = 0; i < delay; i++){
        __NOP();
    }
}