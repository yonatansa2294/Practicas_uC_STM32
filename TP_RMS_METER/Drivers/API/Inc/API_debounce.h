/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_debounce.h
  * @brief          : Header for API_debounce.c file.
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
#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "API_delay.h"	/* <- API_delay include */
/* Exported types ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
#define DEBOUNCE_DELAY	40U	//delay for debounce in milliseconds

/* Public functions -----------------------------------------------------------*/
void debounceFSM_init();
void debounceFSM_update();
bool_t readKey();
bool_t fallingEdge();
bool_t raisingEdge();

#endif /* API_INC_API_DEBOUNCE_H_ */
