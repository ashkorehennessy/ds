/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu6050.h"
#include "ssd1306.h"
#include "UI.h"
#include "sr04.h"
#include "IR.h"
#include "IR.h"
#include "VL53L0X.h"
#include "stepmotor.h"
#include "delay_us.h"
#include "fan.h"
#include "PID.h"
#include "tasks.h"
#include "sercom.h"
#include "tfluna_i2c.h"
#include "beep.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t tof_distance;
statInfo_t_VL53L0X distanceStr;
#define stepmotor_music_notes 35
int last_time = 0;
int stepmotor_music_delay = 250;
int stepmotor_music_frequency[16] = {0, 523, 587, 659, 698, 783, 880, 987, 1046, 1175, 1319, 1397, 1568, 1760, 1976, 2093};
int stepmotor_music_cirno[stepmotor_music_notes] = {2, 3, 4, 5, 6, 6, 6, 6, 4, 6, 4, 3, 2, 2, 2, 2, 0, 2, 5, 6, 9, 9, 9, 9, 8, 9, 8, 5, 6, 6, 6, 0};
int music1[stepmotor_music_notes] = { 10, 10, 12, 13, 13, 13, 12, 13, 12, 10, 9, 12, 10, 10, 10, 10, 10, 0, 10, 10, 12, 13, 13,13, 12, 13, 15, 14, 13,12,13,13,13, 0};
int stepmotor_music_index = 0;
uint32_t perf_time;
double fan_speed = 0;
//PID_Incremental fan_pid;
PID_Base fan_pid;
float pidout;
uint8_t sercom_rx_buf[8];



TF_Luna_Lidar TF_Luna_1;
// device variables passed back by getData
int16_t  tfDist = 0 ;   // distance in centimeters
int16_t  tfFlux = 0 ;   // signal quality in arbitrary units
int16_t  tfTemp = 0 ;   // temperature in 0.01 degree Celsius


int tube_max = 58;
int convert_pos = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

//    // ssd1306显示
    ssd1306_Init();

    // UI界面
    UI_init();
//
    // sr04超声测距
//    sr04.trig_port = GPIOA;
//    sr04.trig_pin = GPIO_PIN_9;
//    sr04.echo_htim = &htim1;
//    sr04.echo_channel = TIM_CHANNEL_1;
//    sr04.use_lowpass_filter = 1;
//    sr04.lowpass_factor = 0.3f;
//    sr04_init(&sr04);


    // vl53l0x tof
//    initVL53L0X(1, &hi2c2);
//
//    // Configure the sensor for high accuracy and speed in 20 cm.
//    setSignalRateLimit(200);
//    setVcselPulsePeriod(VcselPeriodPreRange, 6);
//    setVcselPulsePeriod(VcselPeriodFinalRange, 12);
//    setMeasurementTimingBudget(500 * 1000UL);

    // beep
    beep_init(&beep, &htim2, TIM_CHANNEL_1);

    // 风扇
    fan_init(&fan, &htim3, TIM_CHANNEL_3, TIM_CHANNEL_4);
//    fan_pid = PID_Incremental_Init(-0.05, -0.000025, -0, 55, -55, 0, 0.5);
fan_pid = PID_Base_Init(-0.15, -0.0003, -4.5, 65, -65, 1, 0, 0.5, 39);

    // 串口
    HAL_UART_Receive_IT(&huart2, sercom_rx_buf, 1);

    TF_Luna_init(&TF_Luna_1, &hi2c2, 0x10); //0x10->7bit. 7bit Slave address has been already shifting to the left in the library.

    // tim定时器
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_Delay(500);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      if(stepmotor_music_index >= stepmotor_music_notes){
          stepmotor_music_index = 0;
      }
      beep_set_frequency(&beep, stepmotor_music_frequency[music1[stepmotor_music_index]]);
//      beep_set_frequency(&beep,convert_pos * 100);
      if(HAL_GetTick() - last_time > stepmotor_music_delay){
          stepmotor_music_index++;
          last_time = HAL_GetTick();
      }
      uint32_t perf_start_time = HAL_GetTick();
//      tof_distance = readRangeSingleMillimeters(&distanceStr);
        uint32_t perf_end_time = HAL_GetTick();
        perf_time = perf_end_time - perf_start_time;
        convert_pos = tube_max - tof_distance;
      UI_show();
      UI_key_process();
      if(task_running == 1){
        switch (task_index) {
        case 1:
          task1();
          break;
        case 2:
          task2();
          break;
        case 3:
          task3(input_pos);
          break;
        case 4:
          task4();
          break;
        case 5:
        task5();
        break;
        case 6:
          task6();
          break;
        }
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    uart_queue_push(sercom_rx_buf[0]);
    Ser_pos_callback(&ser_pos);
    HAL_UART_Receive_IT(&huart2, sercom_rx_buf, 1);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
