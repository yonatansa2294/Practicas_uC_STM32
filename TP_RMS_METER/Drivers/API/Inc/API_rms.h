/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_rms.h
  * @brief          : Header for API_rms.c file.
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
#ifndef API_INC_API_RMS_H_
#define API_INC_API_RMS_H_
/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdbool.h>
/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "API_timer.h"	/* <- API_timer include */
#include "API_uart.h"	/* <- API_uart include */
/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;
/* Public define -------------------------------------------------------------*/
#define SUCCESSFUL 		true
#define UNSUCCESSFUL 	false
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Public functions -----------------------------------------------------------*/
bool_t 		rmsFSM_init(TIM_HandleTypeDef *TMRHandle, ADC_HandleTypeDef *ADCHandle, UART_HandleTypeDef *UARTHandle);
void 		rmsFSM_update();
float 		readRMSCurrent();
void 		getSamples();

#endif /* API_INC_API_RMS_H_ */
