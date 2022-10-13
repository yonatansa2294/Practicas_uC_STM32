/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_delay.h
  * @brief          : Header for API_delay.c file.
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
#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/* Private includes ----------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
typedef uint32_t tick_t;
typedef bool bool_t;

/* Structures define ----------------------------------------------------------*/
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

/* Public define -------------------------------------------------------------*/
#define MAX_DELAY	30000	//maximum delay in milliseconds to be implemented

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Public functions -----------------------------------------------------------*/
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );

#endif /* API_INC_API_DELAY_H_ */
