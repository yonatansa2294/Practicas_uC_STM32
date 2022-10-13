/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_timer.h
  * @brief          : Header for API_timer.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef API_INC_API_TIMER_H_
#define API_INC_API_TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/* Private includes ----------------------------------------------------------*/
#include "stm32f4xx_hal.h"	/* <- HAL include */

/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;
/* Public define -------------------------------------------------------------*/
#define SUCCESSFUL 			true
#define UNSUCCESSFUL 		false
#define MAX_COUNTER			65535L	//Maximum timer count value, before overflow -> TIMER10 16bits
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Public functions -----------------------------------------------------------*/
bool_t 					timerInit(TIM_HandleTypeDef *TMRHandle);
bool_t 					timerStart(TIM_HandleTypeDef *TMRHandle);
bool_t 					timerStop(TIM_HandleTypeDef *TMRHandle);

#endif /* API_INC_API_TIMER_H_ */
