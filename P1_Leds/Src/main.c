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

#define DELAY 			200	//delay for turn on LEDs
#define SEQ1  			1	//sequence LED_USER,LED1,LED2
#define SEQ2			2	//sequence LED_USER,LED2,LED1
#define NSEQ			2	//number of sequences
#define COUNTER_MAX		40	//timeout for pushbutton debouncing

uint16_t counter = 0;	//counter of ms for check past bound
uint8_t sequence = 0;
uint8_t state = 0;
uint8_t flag_check = 0;

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all LEDS */
  BSP_LED_Init(LED_USER);
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Initialize push button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  while (1)
  {
	  /* ------------------------------------------ anti-rebound routine ---------------------------*/
	  switch (state)
	  {
	  	  /*-------------------------------------------------------
	  	   * state 0: check if the button has been pressed
	  	   * state 1: delay to check if bounces are gone
	  	   * state 2: check if button is no longer pressed
	  	   * state 3: define which sequence is executed
	  	   * ------------------------------------------------------*/
	  	  case 0:
	  		if(BSP_PB_GetState(BUTTON_USER) == 0)
	  		{
	  			flag_check = 1;	//flag to indicate that the status of the button is being checked.
	  			state = 1;
	  		}

		  break;

	  	  case 1:
	  		  if(counter >= COUNTER_MAX)	//did the waiting time expire?
	  		  {
				state = 2;
				counter = 0;		//reset counter
	  		  }
	  		  else
	  		  {
	  			  counter++;
	  			  HAL_Delay(1);
	  		  }
		  break;

	  	  case 2:
	  		if(BSP_PB_GetState(BUTTON_USER) == 1)	//button pressed
	  			state = 3;
		  break;

	  	  case 3:
	  		sequence ++;
	  		if(sequence > NSEQ)	//condition to reset sequence number to 1. Only sequence values 1 and 2 are considered
	  			sequence = 1;
	  		flag_check = 0;	// check flag released
	  		state = 0;	//return to state 0
		  break;

	  }
	  /* ---------------------------------------------------------------------------------------------------------------*/

	  if(SEQ1 == sequence && flag_check == 0)	// activation sequence  1
	  {
		  BSP_LED_On(LED_USER);
		  HAL_Delay(DELAY);
		  BSP_LED_Off(LED_USER);
		  HAL_Delay(DELAY);

		  BSP_LED_On(LED1);
		  HAL_Delay(DELAY);
		  BSP_LED_Off(LED1);
		  HAL_Delay(DELAY);

		  BSP_LED_On(LED2);
		  HAL_Delay(DELAY);
		  BSP_LED_Off(LED2);
		  HAL_Delay(DELAY);
	  }
	  else if(SEQ2 == sequence && flag_check == 0)	// activation sequence 2
	  {
		  BSP_LED_On(LED_USER);
		  HAL_Delay(DELAY);
		  BSP_LED_Off(LED_USER);
		  HAL_Delay(DELAY);

		  BSP_LED_On(LED2);
		  HAL_Delay(DELAY);
		  BSP_LED_Off(LED2);
		  HAL_Delay(DELAY);

		  BSP_LED_On(LED1);
		  HAL_Delay(DELAY);
		  BSP_LED_Off(LED1);
		  HAL_Delay(DELAY);
	  }
  }
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
