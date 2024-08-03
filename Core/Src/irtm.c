//
// Created by ashkore on 2024/4/29.
//

#include "irtm.h"

IRTM_t irtm;
uint8_t irtm_rx_buf[3];

void IRTM_Init(IRTM_t *irtm_struct, UART_HandleTypeDef *huart) {
    irtm_struct->huart = huart;
    HAL_UART_Receive_IT(irtm_struct->huart, irtm_rx_buf, 3);
}

char IRTM_Read(IRTM_t *irtm_struct) {
    HAL_UART_Receive_IT(irtm_struct->huart, irtm_rx_buf, 3);
    if (irtm_rx_buf[0] == HEADER_FIRST && irtm_rx_buf[1] == HEADER_SECOND) {
        irtm_struct->key_raw = irtm_rx_buf[2];
        return IRTM_KeyToChar(irtm_struct);
    }
    return 0;
}

void IRTM_Send(IRTM_t *irtm_struct, uint8_t key) {
    static uint32_t last_send_time = 0;
    if (HAL_GetTick() - last_send_time < SEND_INTERVAL) {
        return;
    }
    last_send_time = HAL_GetTick();
    uint8_t data[5] = {0xFA, 0xF1, 0x00, 0xFF, key};
    HAL_UART_Transmit(irtm_struct->huart, data, 5, 1000);
}

void IRTM_ChangeBaudrate(IRTM_t *irtm_struct, uint8_t baudrate) {
    static uint32_t last_send_time = 0;
    if (HAL_GetTick() - last_send_time < SEND_INTERVAL) {
        return;
    }
    last_send_time = HAL_GetTick();
    uint8_t data[5] = {0xFA, 0xF3, baudrate, 0x00, 0x00};
    HAL_UART_Transmit(irtm_struct->huart, data, 5, 1000);
}

void IRTM_ChangeAddress(IRTM_t *irtm_struct, uint8_t address) {
    static uint32_t last_send_time = 0;
    if (HAL_GetTick() - last_send_time < SEND_INTERVAL) {
        return;
    }
    last_send_time = HAL_GetTick();
    uint8_t data[5] = {0xFA, 0xF2, address, 0x00, 0x00};
    HAL_UART_Transmit(irtm_struct->huart, data, 5, 1000);
}

char IRTM_KeyToChar(IRTM_t *irtm_struct) {
    switch (irtm_struct->key_raw) {
        case IR_KEY_UP:
            irtm_struct->key = 'U';
            break;
        case IR_KEY_DOWN:
            irtm_struct->key = 'D';
            break;
        case IR_KEY_LEFT:
            irtm_struct->key = 'L';
            break;
        case IR_KEY_RIGHT:
            irtm_struct->key = 'R';
            break;
        case KEY_OK:
            irtm_struct->key = 'O';
            break;
        case KEY_BACK:
            irtm_struct->key = 'B';
            break;
        case KEY_TEST:
            irtm_struct->key = 'T';
            break;
        case KEY_POWER:
            irtm_struct->key = 'P';
            break;
        case KEY_MENU:
            irtm_struct->key = 'M';
            break;
        case KEY_C:
            irtm_struct->key = 'C';
            break;
        case KEY_0:
            irtm_struct->key = '0';
            break;
        case KEY_1:
            irtm_struct->key = '1';
            break;
        case KEY_2:
            irtm_struct->key = '2';
            break;
        case KEY_3:
            irtm_struct->key = '3';
            break;
        case KEY_4:
            irtm_struct->key = '4';
            break;
        case KEY_5:
            irtm_struct->key = '5';
            break;
        case KEY_6:
            irtm_struct->key = '6';
            break;
        case KEY_7:
            irtm_struct->key = '7';
            break;
        case KEY_8:
            irtm_struct->key = '8';
            break;
        case KEY_9:
            irtm_struct->key = '9';
            break;
        default:
            irtm_struct->key = 'N';
            break;
    }
    return irtm_struct->key;
}