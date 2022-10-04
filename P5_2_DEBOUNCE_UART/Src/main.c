/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : First version of program that implements APIs for UART and delay handling.
  * 				  Checks if 0 or 1 is entered to turn on or off the user led of the board.
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
#include "API_delay.h"
#include "API_uart.h"
#include "API_debounce.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

#define DELAY1		100U
#define DELAY2		500U
#define SMS_EDGE	24U
/*---------------------- Definition of variables -----------------------*/
/* UART handler declaration */
UART_HandleTypeDef huart1;
delay_t delayNBLeds;				//structure for delay generation
uint32_t ndelay = DELAY1;
char smsfallingEdge[SMS_EDGE] = "Falling Edge Detected\r\n";
char smsraisingEdge[SMS_EDGE] = "Raising Edge Detected\r\n";

/* Private user code ---------------------------------------------------------*/
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialization of structures */
	delayInit(&delayNBLeds,ndelay);	//initialize structure to implement a delay no blocking
	debounceFSM_init();	// call to function for initialize FSM

	/* Initialize all LEDS */
	BSP_LED_Init(LED_USER);
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);

	/* Initialize push button */
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	/* Initialize UART*/
	if(uartInit(&huart1,9600)== false)
		Error_Handler();

	while (1)
	{

		debounceFSM_update(); //update FSM

		/*we check if any edge has been produced and inform the user with a message on the UART*/
		if(fallingEdge())
			uartSendString(&huart1, (uint8_t *)smsfallingEdge);

		if(raisingEdge())
			uartSendString(&huart1, (uint8_t *)smsraisingEdge);

		/*Check if Key is pressed */
		if (readKey())
		{
			//check delay value
			if(DELAY1 == ndelay)
				ndelay = DELAY2;
			else
				ndelay = DELAY1;

			delayWrite(&delayNBLeds, ndelay);	 //Update delay duration
		}

		if(delayRead(&delayNBLeds))	//check if delay is expired for toggle led2
			BSP_LED_Toggle(LED2);
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

  BSP_LED_On(LED1);
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
