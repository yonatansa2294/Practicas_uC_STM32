/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
//static void MX_GPIO_Init(void);
/* Private user code ---------------------------------------------------------*/
/**
  * @brief  The application entry point.
  * @retval int
  */

/* definition of symbolic constants for the delays of each led */
#define	TIME_LED1	100
#define TIME_LED2	500
#define TIME_LED3	1000

/* Definition of structures for delay generation*/
delay_t delayNbLED1,delayNbLED2,delayNbLED3;

int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialization of structures */
  delayInit(&delayNbLED1,TIME_LED1);	//initialize structure to implement a delay of 100ms
  delayInit(&delayNbLED2,TIME_LED2);	//initialize structure to implement a delay of 500ms
  delayInit(&delayNbLED3,TIME_LED3);	//initialize structure to implement a delay of 1000ms

  /* Initialize all LEDS */
  BSP_LED_Init(LED_USER);
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  while (1)
  {
	  /* check if 100ms have expired, to toggle the status of led */
	  if(delayRead(&delayNbLED1))
	  {
		  BSP_LED_Toggle(LED_USER);
		  delayNbLED1.running = false;	//reset LED1 flag
	  }

	  /* check if 500ms have expired, to toggle the status of led */
	  if(delayRead(&delayNbLED2))
	  {
		  BSP_LED_Toggle(LED1);
		  delayNbLED2.running = false;	//reset LED2 flag
	  }

	  /* check if 1000ms have expired, to toggle the status of led */
	  if(delayRead(&delayNbLED3))
	  {
		  BSP_LED_Toggle(LED2);
		  delayNbLED3.running = false;	//reset LED3 flag
	  }
  }
}

/* structure initialization routine */
void delayInit( delay_t * delay, tick_t duration )
{
	delay->running = false; 	// Initialize flag running
	delay->duration = duration;	// Load the initial value of the delay
}

/* routine to check if the expected time has elapsed */
bool_t delayRead( delay_t * delay )
{
	/* check flag status to start delay calculation */
	if(delay->running == false)
	{
		delay->startTime = HAL_GetTick();	//initial timestamp
		delay->running = true;				// update flag running
		return false;
	}
	else
		/* check if the delay duration has been met */
		return (HAL_GetTick()-delay->startTime)>= delay->duration ? true: false; //returns true if the time has expired
}

/* routine to modify the duration of the delay in the structure */
void delayWrite( delay_t * delay, tick_t duration )
{
	delay->duration = duration;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
static void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  BSP_LED_On(LED_USER);
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