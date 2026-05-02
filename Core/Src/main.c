/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ad9959.h"
#include "arm_math.h"
#include "LMS.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define ADC_BUFFER_SIZE 1024 // 定义ADC缓冲区大小
#define DC_TRACK_ALPHA 0.002f
#define UART_PRINT_DECIM 16U

uint8_t ADC_ConvEnd_1 = 0; // ADC转换完成标志
uint8_t ADC_ConvEnd_2 = 0; // ADC转换完成标志
float p;
uint16_t ADC_Val_1[ADC_BUFFER_SIZE] = {0}; // ADC转换结果缓冲区
uint16_t ADC_Val_2[ADC_BUFFER_SIZE] = {0};
//float single_noise[ADC_BUFFER_SIZE] = {0}; // 存储噪声数据的数组
float out_y[ADC_BUFFER_SIZE] = {0}; // 存储输出数据的数组
float out_error[ADC_BUFFER_SIZE] = {0}; // 存储误差数据的数组
uint16_t lms_x_his[ADC_BUFFER_SIZE];
float lms_ref_block[ADC_BUFFER_SIZE] = {0};
float lms_ref_tail[LMS_M - 1] = {0}; // 保存上一块末尾参考输入，避免块边界瞬态
float ref_dc_est = 0.0f;
float d_dc_est = 0.0f;
float noise[ADC_BUFFER_SIZE] ={0 
}; // 存储噪声数据的数组
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance == ADC1)
  {
    ADC_ConvEnd_1 = 1;//设置转换完成标志
  }
  if(hadc->Instance == ADC2)
  {
    ADC_ConvEnd_2 = 1;//设置转换完成标志
  }
}
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Init(&htim3);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Val_1, ADC_BUFFER_SIZE);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)ADC_Val_2, ADC_BUFFER_SIZE);
  HAL_TIM_Base_Start(&htim3);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
    if(ADC_ConvEnd_1 && ADC_ConvEnd_2)
    {
        ADC_ConvEnd_1 = 0;
        ADC_ConvEnd_2 = 0;	
				memset((void *)lms_x, 0, sizeof(lms_x));

        for(uint16_t i = 0; i < ADC_BUFFER_SIZE; i++)
        {
          float x_raw = (float)ADC_Val_2[i] / 4095.0f;
          // ref_dc_est += DC_TRACK_ALPHA * (x_raw - ref_dc_est);
          lms_ref_block[i] = x_raw; //- ref_dc_est;
        }
			
        for(uint16_t i = 0; i < ADC_BUFFER_SIZE; i++)
        {
          for(uint16_t j = 0; j < LMS_M; j++)
          {
            if(i >= j)
            {
              lms_x[j] = lms_ref_block[i - j];
            }
            else
            {
              uint16_t miss = j - i;
              lms_x[j] = lms_ref_tail[(LMS_M - 1U) - miss];
            }
          }

          float d_raw = (float)ADC_Val_1[i] / 4095.0f;
          d_dc_est += DC_TRACK_ALPHA * (d_raw - d_dc_est);
          lms_input.d = d_raw;// - d_dc_est;
          lms_input.x_ptr = lms_x;
          lms_input.lenth_x = LMS_M;

          LMS_Theory(&lms_input, &lms_output);

          out_y[i] = lms_output.y;
          out_error[i] = lms_output.error;

          Usart1Printf("%f,%f,%f,%f\r\n", out_y[i], out_error[i], d_raw, lms_ref_block[i]);


        }

        for(uint16_t k = 0; k < (LMS_M - 1U); k++)
        {
          lms_ref_tail[k] = lms_ref_block[ADC_BUFFER_SIZE - (LMS_M - 1U) + k];
        }

        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Val_1, ADC_BUFFER_SIZE);
        HAL_ADC_Start_DMA(&hadc2, (uint32_t *)ADC_Val_2, ADC_BUFFER_SIZE);
    }
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
#ifdef USE_FULL_ASSERT
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
