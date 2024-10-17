//
// Created by ashkore on 2024/4/12.
//

#include "tasks.h"
#include "PID.h"
#include "beep.h"

float A_pos = 4;
float B_pos = 17;
float C_pos = 35;
float D_pos = 48;
float pos_offset = 0;
float target_pos = 0;
uint8_t task_running = 0;
float task_running_time = 0;
float task_start_running_time = 0;
uint8_t task_index = 7;
float input_pos = 0;
uint32_t keep_time = 0;
float keep_time_sec = 0;

extern uint16_t tof_distance;
extern PID_Base fan_pid;
extern float pidout;

#define stepmotor_music_notes 128
extern int stepmotor_music_delay;
extern int stepmotor_music_frequency[16];
extern int stepmotor_music_cirno[stepmotor_music_notes];
extern int music1[stepmotor_music_notes];
extern int music2[stepmotor_music_notes];
extern int music3[stepmotor_music_notes];
extern int stepmotor_music_index;
extern int music_senrenbanka[stepmotor_music_notes];

int task1(){
    // keep ball between B and C above 5s

    static uint32_t last_time = 0;
    target_pos = (B_pos + C_pos) / 2 + pos_offset;
    uint32_t target_keep_time = 6000;
    if(tof_distance > B_pos && tof_distance < C_pos){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        last_time = 0;
        task_running = 0;
        fan_pid.integral = 0;
        return 1;
    } else {
        return 0;
    }
}

int task2(){
    // keep ball between B and C above 10s

    static uint32_t last_time = 0;
    target_pos = (B_pos + C_pos) / 2 + pos_offset;
    uint32_t target_keep_time = 100000;
    if(tof_distance > B_pos && tof_distance < C_pos){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        last_time = 0;
        task_running = 0;
        fan_pid.integral = 0;
        return 1;
    } else {
        return 0;
    }
}

int task3(float _input_pos){
    //Take the coordinates of point C as 0cm and the coordinates of point B as 15cm; use the keyboard to set the height position of the ball (unit: cm). After starting, the ball should be stably at the specified height for more than 3 seconds.

    static uint32_t last_time = 0;
    if(_input_pos >= 0) {
        _input_pos = _input_pos * 17.4 / 15;
    }
    target_pos = C_pos - _input_pos + pos_offset;
    uint32_t target_keep_time = 5000;
    if(tof_distance > target_pos - 1.5 && tof_distance < target_pos + 1.5){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        last_time = 0;
        task_running = 0;
        fan_pid.integral = 0;
        return 1;
    } else {
        return 0;
    }
}

int task4(){
    //The small ball is placed at the bottom of the round tube. Within 5 seconds after starting, the small ball is controlled to reach the end A at the top of the round tube without jumping away and maintains for 5 seconds.

    static uint32_t last_time = 0;
    static uint32_t task_start_time = 0;
    uint32_t target_keep_time = 6000;
    if(task_start_time == 0){
        task_start_time = HAL_GetTick();
    }
    if(HAL_GetTick() - task_start_time < 800){
        target_pos = (A_pos + B_pos) / 2 + pos_offset;
    } else {
        target_pos = A_pos - 5;
    }
    if(tof_distance < A_pos + 2){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        task_running = 0;
        last_time = 0;
        fan_pid.integral = 0;
        task_start_time = 0;
        return 1;
    } else {
        return 0;
    }
}

int task5(){
    static uint32_t last_time = 0;
    static uint32_t task_start_time = 0;
    uint32_t target_keep_time = 500;
    target_pos = A_pos - 1000;
    if(tof_distance > D_pos && last_time != 0){
        keep_time += HAL_GetTick() - last_time;
    } else {
        keep_time = 0;
    }
    last_time = HAL_GetTick();
    if(keep_time > target_keep_time){
        task_running = 0;
        fan_pid.integral = 0;
        task_start_time = 0;
        last_time = 0;
        return 1;
    } else {
        return 0;
    }
}

int task6(){
    // The small ball is placed at the bottom of the round tube. Within 30 seconds after starting, the small ball is controlled to complete the following movements: upward to reach segment AB and maintain for 3 to 5 seconds, then downward to reach segment CD and maintain for 3 to 5 seconds; then upward to reach segment AB and maintain

    static uint32_t last_time = 0;
    static uint32_t task_start_time = 0;
    static uint32_t task_end_time = 0;
    static uint32_t task_time = 0;
    static uint8_t task_state = 0;
    static uint32_t part_start_time = 0;
    if(task_state == 0){
        if(part_start_time == 0){
            part_start_time = HAL_GetTick();
        }
        if(HAL_GetTick() - part_start_time < 1200) {
            target_pos = A_pos;
        } else {
            target_pos = (A_pos + B_pos) / 2 + pos_offset;
        }
        target_pos = (A_pos + B_pos) / 2 + pos_offset;
        uint32_t target_keep_time = 3700;
        if(tof_distance < B_pos && tof_distance > A_pos){
            keep_time += HAL_GetTick() - last_time;
        } else {
            keep_time = 0;
        }
        last_time = HAL_GetTick();
        if(keep_time > target_keep_time){
            task_state = 1;
            part_start_time = 0;
            task_start_time = HAL_GetTick();
        }
    } else if(task_state == 1){
        target_pos = (C_pos + D_pos) / 2 + pos_offset;
        uint32_t target_keep_time = 3700;
        if(tof_distance < D_pos && tof_distance > C_pos){
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
        target_pos = (A_pos + B_pos) / 2;
        uint32_t target_keep_time = 3700;
        if(tof_distance < B_pos && tof_distance > A_pos){
            keep_time += HAL_GetTick() - last_time;
        } else {
            part_start_time = 0;
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
        uint32_t target_keep_time = 3700;
        if(tof_distance < D_pos && tof_distance > C_pos){
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
        target_pos = A_pos - 1000;
        uint32_t target_keep_time = 500;
        if (tof_distance > D_pos){
            keep_time += HAL_GetTick() - last_time;
        } else {
            keep_time = 0;
        }
        last_time = HAL_GetTick();
        if (keep_time > target_keep_time) {
            last_time = 0;
            task_running = 0;
            task_state = 0;
            fan_pid.integral = 0;
            task_start_time = 0;
            fan_pid.use_integral = 1;
            return 5;
        }
    }
    return 0;
}

int task7(){
    // let ball follow the music to move

    static uint32_t last_time = 0;
    static uint32_t task_start_time = 0;
    static uint32_t task_end_time = 0;
    static uint32_t task_time = 0;
    static uint8_t task_state = 0;
    static uint32_t part_start_time = 0;
    if(part_start_time == 0){
        part_start_time = HAL_GetTick();
    }
    target_pos = 45 - (music_senrenbanka[stepmotor_music_index] * 5 - 30);
    if(HAL_GetTick() - part_start_time > 180){
        beep_set_frequency(&beep, stepmotor_music_frequency[music_senrenbanka[stepmotor_music_index]]);
        part_start_time = 0;
        stepmotor_music_index++;
    }
    if (stepmotor_music_index >= stepmotor_music_notes) {
        last_time = 0;
        task_running = 0;
        task_state = 0;
        fan_pid.integral = 0;
        task_start_time = 0;
        stepmotor_music_index = 0;
        return 6;
    }
    return 0;
}