//
// Created by ashkore on 2024/4/12.
//

#ifndef DS_TASKS_H
#define DS_TASKS_H

#include "stm32f1xx_hal.h"

extern float A_pos;
extern float B_pos;
extern float C_pos;
extern float D_pos;
extern float pos_offset;
extern float pos_convert;
extern float target_pos;
extern uint8_t task_running;
extern uint8_t task_index;
extern float input_pos;
extern uint32_t keep_time;
extern float keep_time_sec;
extern float task_running_time;
extern float task_start_running_time;

int task1();

int task2();

int task3(float _input_pos);

int task4();

int task5();

int task6();

int task7();

#endif //DS_TASKS_H
