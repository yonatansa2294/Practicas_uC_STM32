/* USER CODE BEGIN Header */
/**
  **************************************************************************************************
  * @file           : API_delay.c
  * @brief          : API for implementing non-blocking delays in milliseconds, using SysTick Timer
  * 				  The implemented functions are:
  * 				  	1.delayInit
  *  			 		2.delayWrite
  *  			 		3.delayRead
  *                   This file contains the common defines of the application.
  * Created on		: Oct 10, 2022
  *  Author			: Yonatan Aguirre
  **************************************************************************************************
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

#include "API_delay.h"

/**
 * @brief:  Load the delay duration value into the structure. Also, initialize the running flag to 'false'.
 * 			This function does not initialize the delay count.
 * @param1: delay pointer to the delay_t structure.
 * @param2: duration pointer to the tick_t structure.
 * @retval: none
 */
void delayInit( delay_t * delay, tick_t duration )
{
	/*------------------------------- verification of parameters -------------------------*/
	if(duration > MAX_DELAY)	//check if the delay value is higher than the allowed value
		duration = MAX_DELAY;

	if(delay != NULL)			//verify if the address of the structure is valid
	{
		delay->running = false; 	// Initialize flag running
		delay->duration = duration;	// Load the initial value of the delay
	}
}

/**
 * @brief:  Checks if the delay was met. Returns a true value when the condition is met.
 * 			When the time is expired, the running flag is changed to false.
 * @param1: delay pointer to the delay_t structure.
 * @retval: none
 */
bool_t delayRead( delay_t * delay )
{
	bool toggleLed = false;	//flag to indicated when the led is to be toggle

	/*-------- check flag status to start delay calculation -------*/
	if(delay->running == false)
	{
		delay->startTime = HAL_GetTick();	//initial timestamp
		delay->running = true;				// update flag running
	}
	else
	{
		/*------------- check if the delay duration has been met ----------*/
		if((HAL_GetTick()-delay->startTime)>= delay->duration)
		{
			delay->running = false;	//reset flag of start
			toggleLed = true;	//returns true if the time has expired
		}
	}

	return toggleLed;
}

/**
 * @brief:  Modifies the duration time of an existing delay.
 * @param1: delay pointer to the delay_t structure.
 * @param2: duration pointer to the tick_t structure.
 * @retval: none
 */
void delayWrite( delay_t * delay, tick_t duration )
{
	/* verification of parameters */
	if(duration > MAX_DELAY)
		duration = MAX_DELAY;

	if(delay != NULL)
		delay->duration = duration;
}

