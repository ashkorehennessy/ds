/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu6050.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define is_nan(x) ((x) != (x))
#define abs(x) ((x) > 0 ? (x) : -(x))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim4;
/* USER CODE BEGIN EV */
double angle_queue[10];
double angleZ_queue[10];
double x_move_queue[10];
double y_move_queue[10];
double accx_queue[30];
double accy_queue[30];
double angx_queue[30];
double angy_queue[30];
double gyrox_queue[30];
double gyroy_queue[30];
double number_queue[10];
extern char stand;
extern char down;
extern double angle;
extern char trend;
extern char x_axis;
extern char y_axis;
extern char number;
extern int length;
char number_direction = ' ';
int flag = 0;
int count = 0;
double accx_average;
double accy_average;
double derivative;
uint32_t skip_detect = 0;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
    MPU6050_Read_All(&hi2c2, &mpu6050);
    queue_push(accx_queue, mpu6050.Ax, 30);
    queue_push(accy_queue, mpu6050.Ay, 30);
    queue_push(angx_queue, mpu6050.KalmanAngleX, 30);
    queue_push(angy_queue, mpu6050.KalmanAngleY, 30);
    queue_push(gyrox_queue, mpu6050.Gx, 30);
    queue_push(gyroy_queue, mpu6050.Gy, 30);
    angle = 90 - sqrt(mpu6050.KalmanAngleX * mpu6050.KalmanAngleX + mpu6050.KalmanAngleY * mpu6050.KalmanAngleY);
    if(angle > 50){
        stand = '^';
        down = ' ';
    } else {
        stand = ' ';
        down = 'v';
    }
    queue_push(angle_queue, angle, 10);
    derivative = queue_derivative(angle_queue, 10);
    if(derivative > 1){
        trend = '+';
    } else if(derivative < -1) {
        trend = '-';
    } else {
        trend = ' ';
    }

    double derivative_angx = queue_derivative(angx_queue, 30);
    double derivative_angy = queue_derivative(angy_queue, 30);
    double derivative_gyrox = queue_derivative(gyrox_queue, 30);
    double derivative_gyroy = queue_derivative(gyroy_queue, 30);
    if(abs(derivative_angx) > 25 && abs(derivative_angy) < 12){
        x_axis = '*';
        y_axis = ' ';
    } else if(abs(derivative_angy) > 25 && abs(derivative_angx) < 12){
        x_axis = ' ';
        y_axis = '*';
    }
    if(HAL_GetTick() < 3000){
        x_axis = ' ';
        y_axis = ' ';
        number = ' ';
    }

    count++;
    if(count == 12){
        queue_push(angleZ_queue, mpu6050.AngleZ, 10);
        count = 0;
    }

    if(HAL_GetTick() - skip_detect > 1000) {
        if (derivative_angx > 10 && derivative_gyrox > 100 && number_direction != 'R') {
            number_direction = 'R';
            queue_push(number_queue, number_direction, 10);
            skip_detect = HAL_GetTick();
        } else if (derivative_angx < -10 && derivative_gyrox < -100 && number_direction != 'L') {
            number_direction = 'L';
            queue_push(number_queue, number_direction, 10);
            skip_detect = HAL_GetTick();
        } else if (derivative_angy > 10 && derivative_gyroy > 100 && number_direction != 'U') {
            number_direction = 'U';
            queue_push(number_queue, number_direction, 10);
            skip_detect = HAL_GetTick();
        } else if (derivative_angy < -10 && derivative_gyroy < -100 && number_direction != 'D') {
            number_direction = 'D';
            queue_push(number_queue, number_direction, 10);
            skip_detect = HAL_GetTick();
        }
    }
    number = number_recognize(number_queue, 10);

    derivative = queue_derivative(angleZ_queue, 10);
    if(derivative > 120 && flag == 0){
        flag = 1;
    } else if(derivative < -120 && flag == 1){
        flag = 0;
        NVIC_SystemReset();
    }


  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void queue_push(double *queue, double _angle, int size) {
    for(int i = 0; i < size - 1; i++){
        queue[i] = queue[i + 1];
    }
    queue[size - 1] = _angle;
}
double queue_derivative(double *queue, int size) {
    return (queue[size - 1] - queue[0]);
}
double queue_average(const double *queue, int size) {
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += queue[i];
    }
    return sum / size;
}
char number_recognize(const double *queue, int size) {
    if(queue[7] == 'U' && queue[8] == 'R' && queue[9] == 'D') {
        return '4';
    } else if(queue[8] == 'R' && queue[9] == 'D') {
        return '7';
    } else if(queue[5] == 'R' && queue[6] == 'D' && queue[7] == 'L' && queue[8] == 'D' && queue[9] == 'R'){
        return '2';
    } else if(queue[5] == 'L' && queue[6] == 'U' && queue[7] == 'R' && queue[8] == 'D' && queue[9] == 'L'){
        return '9';
    } else if(queue[5] == 'L' && queue[6] == 'D' && queue[7] == 'R' && queue[8] == 'D' && queue[9] == 'L'){
        return '5';
    } else if(queue[5] == 'R' && queue[6] == 'D' && queue[7] == 'L' && queue[8] == 'D' && queue[9] == 'L'){
        return '3';
    } else if(queue[5] == 'L' && queue[6] == 'D' && queue[7] == 'R' && queue[8] == 'U' && queue[9] == 'L'){
        return '6';
    } else if(queue[3] == 'U' && queue[4] == 'R' && queue[5] == 'D' &&queue[6] == 'L' && queue[7] == 'D' && queue[8] == 'R' && queue[9] == 'U'){
        return '8';
    } else if(queue[9] == 'D'){
        return '1';
    } else {
        return ' ';
    }
}
/* USER CODE END 1 */
