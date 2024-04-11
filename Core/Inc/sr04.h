//
// Created by ashkore on 23-9-2.
//

#include "stm32f1xx_hal.h"

#ifndef SR04_SR04_H
#define SR04_SR04_H

#endif //SR04_SR04_H

// HC-SR04 struct
typedef struct {
    GPIO_TypeDef *trig_port;  // Trigger pin dir_port
    uint16_t trig_pin;  // Trigger pin number
    TIM_HandleTypeDef *echo_htim;  // Echo pin timer
    uint16_t echo_channel;  // Echo pin timer channel
    uint8_t capture_flag;  // Echo pin capture flag
    uint32_t distance;  // Distance in mm
    uint32_t last_distance;  // Last distance in mm
    uint16_t tim_update_count;  // Timer update count
    uint8_t use_lowpass_filter;  // Use low pass filter
    float lowpass_factor;  // Low pass filter factor
} sr04_t;

extern sr04_t sr04;

// HC-SR04 functions
void sr04_init(sr04_t *sr04_struct);
void sr04_trigger(sr04_t *sr04_struct);
void sr04_read_distance(sr04_t *sr04_struct);  // This function should be called in the timer input capture callback
