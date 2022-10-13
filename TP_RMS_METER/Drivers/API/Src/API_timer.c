/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_timer.c
  * @brief          : API for Timer10 interrupt configuration and handling.
  * 				  The implemented functions are:
  *  			 		1.uartInit
  *  			 		2.timerStart
  *  			 		3.timerStop
  *                   This file contains the common defines of the application.
  * Created on		: Oct 10, 2022
  *  Author			: Yonatan Aguirre
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

#include "API_timer.h"

/**
 * @brief:  Set the Timer to generate an overflow interrupt, every 4.16ms
 * @param1: TMRHandle pointer to the TIM_HandleTypeDef
 * @retval: none
 */
bool_t timerInit(TIM_HandleTypeDef *TMRHandle)
{
	bool_t configStatus=UNSUCCESSFUL;

	/*************************************************************************************
	 * Calculation of the period:
	 * Clock frequency (Fclk) : 60MHz
	 * Prescaler(PSC)         : 60
	 * Sampling frequency (Fs): 240Hz
	 *
	 *     		 Clk_CNT		10^6
	 * Period = --------- =  --------- = 4167
	 *             Fs	        240
	 *
	 * We set its value to 4166 since we want the interrupt to be triggered on roll over
	 * -----------------------------------------------------------------------------------
	 * Calculation of Prescaler:
	 * The Prescaler value is defined by the PSC[15:0] register
	 * but the counter clock is calculated as follows:
	 *				  Fclk
	 * Clk_cnt =   -------------
	 * 				PSC[15:0]+1
	 *
	 * We write the value as "60 - 1" to show that a prescaler value of 59 actually
	 * means using a clock divider or 60.
	 * **********************************************************************************/

	/* USER CODE END TIM10_Init 1 */
	if (TMRHandle =! NULL)
	{
		TMRHandle->Instance = TIM10;
		TMRHandle->Init.Prescaler = 60 - 1;
		TMRHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
		TMRHandle->Init.Period = 4166;
		TMRHandle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		TMRHandle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

		if (HAL_TIM_Base_Init(TMRHandle) == HAL_OK)
			configStatus = SUCCESSFUL;
		else
			configStatus = UNSUCCESSFUL;
	}

	return configStatus;
}

/**
 * @brief:  Starts Timer10 interruption
 * @param1: TMRHandle pointer to the TIM_HandleTypeDef
 * @retval: none
 */
bool_t timerStart(TIM_HandleTypeDef *TMRHandle)
{
	bool_t startTMR = UNSUCCESSFUL;

	if (TMRHandle =! NULL)
	{
		if(HAL_TIM_Base_Start_IT(TMRHandle) == HAL_OK)
			startTMR = SUCCESSFUL;
		else
			startTMR = UNSUCCESSFUL;
	}

	return startTMR;
}

/**
 * @brief:  Stop Timer10 interruption
 * @param1: TMRHandle pointer to the TIM_HandleTypeDef
 * @retval: none
 */
bool_t timerStop(TIM_HandleTypeDef *TMRHandle)
{
	bool_t stopTMR=UNSUCCESSFUL;

	if (TMRHandle =! NULL)
	{
		if(HAL_TIM_Base_Stop_IT(TMRHandle) == HAL_OK)
			stopTMR = SUCCESSFUL;
		else
			stopTMR = UNSUCCESSFUL;
	}

	return stopTMR;
}
