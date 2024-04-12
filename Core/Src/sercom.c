//
// Created by ashkore on 2024/4/12.
//

#include "sercom.h"

uint8_t uart_queue[8];

Ser_pos ser_pos;

void uart_queue_push(uint8_t data)
{
    for (int i = 0; i < 7; i++)
    {
        uart_queue[i] = uart_queue[i + 1];
    }
    uart_queue[7] = data;
}

void uart_queue_clear()
{
    for (int i = 0; i < 8; i++)
    {
        uart_queue[i] = 0;
    }
}

void Ser_pos_callback(Ser_pos *pos)
{
    if(uart_queue[0] == SERCOM_HEAD1 && uart_queue[1] == SERCOM_HEAD2 && uart_queue[6] == SERCOM_TAIL1 && uart_queue[7] == SERCOM_TAIL2)
    {
        pos->X = (int16_t)(uart_queue[2] << 8 | uart_queue[3]);
        pos->Y = (int16_t)(uart_queue[4] << 8 | uart_queue[5]);
    }
}