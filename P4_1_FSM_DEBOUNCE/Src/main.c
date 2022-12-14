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
#include "API_delay.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

void debounceFSM_init();
void debounceFSM_update();
void buttonPressed();
void buttonReleased();

/* Private user code ---------------------------------------------------------*/
/*-------------------- Definition of variables and constant -----------------------*/
#define DEBOUNCE_DELAY	40U	//delay for rebound

/*----------------------- Global Variables ---------------------*/
/* Enumeration of states for FSM*/
typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

delay_t delayNbLED;				//structure for delay generation
debounceState_t fsmButtonState;

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

  /* Initialize all LEDS and button */
  BSP_LED_Init(LED_USER);
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  debounceFSM_init();	// call to function for initialize FSM


  while (1)
  {
	  debounceFSM_update(); //update FSM
  }
}

/**
  * @brief  Initilizes the FSM with the initial state of the unpressed button
  * @param  None
  * @retval None
  */
void debounceFSM_init()
{
	fsmButtonState = BUTTON_UP;	//initial state
	delayInit(&delayNbLED,DEBOUNCE_DELAY);	//initialize structure to implement a delay of 40ms
}

/**
  * @brief  Reads the state of the user button and implements state transition logic to update the outputs
  * by triggering events buttonPressed y buttonReleased
  * @param  None
  * @retval None
  */
void debounceFSM_update()
{
	switch (fsmButtonState)
	{
		case BUTTON_UP:	//initial state
			if(!BSP_PB_GetState(BUTTON_USER))
			{
				fsmButtonState = BUTTON_FALLING;	//falling edge detected
				delayRead(&delayNbLED);				//initialize delay
			}

			break;

		case BUTTON_FALLING:
			if(delayRead(&delayNbLED))	// check if the anti-rebound delay has expired
			{
				if(!BSP_PB_GetState(BUTTON_USER))	//new reading to check if the button was pressed
				{
					fsmButtonState = BUTTON_DOWN;
					buttonPressed();	//valid button press -> call to button pressed event
				}
				else
					fsmButtonState = BUTTON_UP;	// button is not pressed, return to initial state
			}
			break;

		case BUTTON_DOWN:
			if(BSP_PB_GetState(BUTTON_USER))
			{
				fsmButtonState = BUTTON_RAISING;	//rising edge detected
				delayRead(&delayNbLED);				//initialize delay
			}
			break;

		case BUTTON_RAISING:
			if(delayRead(&delayNbLED))	// check if the anti-rebound delay has expired
			{
				if(BSP_PB_GetState(BUTTON_USER))	//new reading to check if the button is not pressed
				{
					fsmButtonState = BUTTON_UP;
					buttonReleased();	//valid button released -> call to button released event
				}
				else
					fsmButtonState = BUTTON_DOWN; // button still pressed, return to button down state
			}
			break;

		default:
			Error_Handler();
			break;
	}
}

/**
  * @brief  Event generated when validating that the button is pressed. The status of LED1 is toggled
  * @param  None
  * @retval None
  */
void buttonPressed()
{
	BSP_LED_Toggle(LED1);
}

/**
  * @brief  Event generated when validating that the button is no longer pressed. The status of LED2 is toggled
  * @param  None
  * @retval None
  */
void buttonReleased()
{
	BSP_LED_Toggle(LED2);
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
