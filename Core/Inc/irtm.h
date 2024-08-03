//
// Created by ashkore on 2024/4/29.
//

#ifndef DS_IRTM_H
#define DS_IRTM_H

#include "stm32f1xx_hal.h"

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t key_raw;
    char key;
} IRTM_t;

extern IRTM_t irtm;

void IRTM_Init(IRTM_t *irtm_struct, UART_HandleTypeDef *huart);

char IRTM_Read(IRTM_t *irtm_struct);

void IRTM_Send(IRTM_t *irtm_struct, uint8_t key);

void IRTM_ChangeBaudrate(IRTM_t *irtm_struct, uint8_t baudrate);

void IRTM_ChangeAddress(IRTM_t *irtm_struct, uint8_t address);

char IRTM_KeyToChar(IRTM_t *irtm_struct);


// IR remote control settings
#define SEND_INTERVAL 300
#define HEADER_FIRST 0x00
#define HEADER_SECOND 0xFF
#define CONFIG_BAUDRATE_4800 0x01
#define CONFIG_BAUDRATE_9600 0x02
#define CONFIG_BAUDRATE_19200 0x03
#define CONFIG_BAUDRATE_57600 0x04

// Key codes
#define IR_KEY_UP 0x40
#define IR_KEY_DOWN 0x19
#define IR_KEY_LEFT 0x07
#define IR_KEY_RIGHT 0x09
#define KEY_OK 0x15
#define KEY_BACK 0x43
#define KEY_TEST 0x44
#define KEY_POWER 0x45
#define KEY_MENU 0x47
#define KEY_MUTE 0x47
#define KEY_MODE 0x46
#define KEY_C 0x0D
#define KEY_0 0x16
#define KEY_1 0x0C
#define KEY_2 0x18
#define KEY_3 0x5E
#define KEY_4 0x08
#define KEY_5 0x1C
#define KEY_6 0x5A
#define KEY_7 0x42
#define KEY_8 0x52
#define KEY_9 0x4A

#endif //DS_IRTM_H
