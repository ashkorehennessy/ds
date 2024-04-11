//
// Created by ashkore on 2024/4/11.

/**
 * @file    softi2c_conf.h
 * @author  Myth
 * @version 0.1
 * @date    2021.10.12
 * @brief   STM32 SoftI2C Library Config File
 */

#ifndef __SOFTI2C_CONF_H
#define __SOFTI2C_CONF_H

// Set your HAL Library here.

#include "stm32f1xx_hal.h"

// Set your owm Delay_us function here.

#include "delay_us.h"

#define SoftI2C_Delay_us(__time__) Delay_us(__time__)

#endif