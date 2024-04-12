//
// Created by ashkore on 2024/4/12.
//

#include "tasks.h"

float A_pos = 10;
float B_pos = 25;
float C_pos = 40;
float D_pos = 55;
float pos_offset = 0;
float pos_convert = 50;
float target_pos = 0;
uint8_t task_running = 0;
uint8_t task_index = 0;
int input_pos = 0;

extern uint16_t vl53l0x_distance;

int task1(){
    // keep ball between B and C above 5s
    static uint32_t keep_time = 0;
    static uint32_t last_time = 0;
    float pos =  pos_convert - vl53l0x_distance;
    target_pos = (B_pos + C_pos) / 2 + pos_offset;
    uint32_t target_keep_time = 5000;
    if(pos > B_pos && pos < C_pos){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        task_running = 0;
        return 1;
    } else {
        return 0;
    }
}

int task2(){
    // keep ball between B and C above 10s
    static uint32_t keep_time = 0;
    static uint32_t last_time = 0;
    float pos =  pos_convert - vl53l0x_distance;
    target_pos = (B_pos + C_pos) / 2 + pos_offset;
    uint32_t target_keep_time = 10000;
    if(pos > B_pos && pos < C_pos){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        task_running = 0;
        return 1;
    } else {
        return 0;
    }
}

int task3(int input_pos){
    //Take the coordinates of point C as 0cm and the coordinates of point B as 15cm; use the keyboard to set the height position of the ball (unit: cm). After starting, the ball should be stably at the specified height for more than 3 seconds.
    static uint32_t keep_time = 0;
    static uint32_t last_time = 0;
    float pos =  pos_convert - vl53l0x_distance;
    target_pos = C_pos - input_pos + pos_offset;
    uint32_t target_keep_time = 3000;
    if(pos > target_pos - 1 && pos < target_pos + 1){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        task_running = 0;
        return 1;
    } else {
        return 0;
    }
}

int task4(){
    //The small ball is placed at the bottom of the round tube. Within 5 seconds after starting, the small ball is controlled to reach the end A at the top of the round tube without jumping away and maintains for 5 seconds.
    static uint32_t keep_time = 0;
    static uint32_t last_time = 0;
    float pos =  pos_convert - vl53l0x_distance;
    target_pos = A_pos + pos_offset;
    uint32_t target_keep_time = 5000;
    if(pos > A_pos - 1 && pos < A_pos + 1){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        task_running = 0;
        return 1;
    } else {
        return 0;
    }
}

int task6(){
    // The small ball is placed at the bottom of the round tube. Within 30 seconds after starting, the small ball is controlled to complete the following movements: upward to reach segment AB and maintain for 3 to 5 seconds, then downward to reach segment CD and maintain for 3 to 5 seconds; then upward to reach segment AB and maintain
    static uint32_t keep_time = 0;
    static uint32_t last_time = 0;
    static uint32_t task_start_time = 0;
    static uint32_t task_end_time = 0;
    static uint32_t task_time = 0;
    static uint8_t task_state = 0;
    float pos =  pos_convert - vl53l0x_distance;
    if(task_state == 0){
        target_pos = (A_pos + B_pos) / 2 + pos_offset;
        uint32_t target_keep_time = 5000;
        if(pos > B_pos && pos < A_pos){
            keep_time += HAL_GetTick() - last_time;
        } else {
            keep_time = 0;
        }
        last_time = HAL_GetTick();
        if(keep_time > target_keep_time){
            task_state = 1;
            task_start_time = HAL_GetTick();
        }
    } else if(task_state == 1){
        target_pos = (C_pos + D_pos) / 2 + pos_offset;
        uint32_t target_keep_time = 5000;
        if(pos > D_pos && pos < C_pos){
            keep_time += HAL_GetTick() - last_time;
        } else {
            keep_time = 0;
        }
        last_time = HAL_GetTick();
        if(keep_time > target_keep_time){
            task_state = 2;
            task_end_time = HAL_GetTick();
            task_time = task_end_time - task_start_time;
        }
    } else if(task_state == 2){
        target_pos = (A_pos + B_pos) / 2 + pos_offset;
        uint32_t target_keep_time = 5000;
        if(pos > B_pos && pos < A_pos){
            keep_time += HAL_GetTick() - last_time;
        } else {
            keep_time = 0;
        }
        last_time = HAL_GetTick();
        if(keep_time > target_keep_time){
            task_state = 3;
            task_end_time = HAL_GetTick();
            task_time = task_end_time - task_start_time;
        }
    } else if(task_state == 3){
        target_pos = (C_pos + D_pos) / 2 + pos_offset;
        uint32_t target_keep_time = 5000;
        if(pos > D_pos && pos < C_pos){
            keep_time += HAL_GetTick() - last_time;
        } else {
            keep_time = 0;
        }
        last_time = HAL_GetTick();
        if(keep_time > target_keep_time){
            task_state = 4;
            task_end_time = HAL_GetTick();
            task_time = task_end_time - task_start_time;
        }
    } else if(task_state == 4) {
        target_pos = A_pos - 10 + pos_offset;
        uint32_t target_keep_time = 2000;
        if (pos > A_pos - 10){
            keep_time += HAL_GetTick() - last_time;
        } else {
            keep_time = 0;
        }
        last_time = HAL_GetTick();
        if (keep_time > target_keep_time) {
            task_running = 0;
            return 5;
        }
    }
    return 0;
}