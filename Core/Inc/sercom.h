//
// Created by ashkore on 2024/4/12.
//

#ifndef DS_SERCOM_H
#define DS_SERCOM_H

#include "stm32f1xx_hal.h"

#define SERCOM_HEAD1 0x55
#define SERCOM_HEAD2 0xAA
#define SERCOM_TAIL1 0x0D
#define SERCOM_TAIL2 0x0A

typedef struct {
    int16_t X;
    int16_t Y;
} Ser_pos;

extern Ser_pos ser_pos;

extern uint8_t uart_queue[8];

void uart_queue_push(uint8_t data);

void uart_queue_clear();

void Ser_pos_callback(Ser_pos *pos);

#endif //DS_SERCOM_H
