/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_adc.h
  * @brief          : Header for API_adc.c file.
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
#ifndef API_INC_API_ADC_H_
#define API_INC_API_ADC_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/* Private includes ----------------------------------------------------------*/
#include "stm32f4xx_hal.h"	/* <- HAL include */
#include "API_uart.h"		/* <- API_uart include */

/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;
/* Public define -------------------------------------------------------------*/
#define SUCCESSFUL 			true
#define UNSUCCESSFUL 		false
#define MAX_VALUE_12B		4095L
#define MAX_VALUE_10B		1023L
#define MAX_VALUE_8B		255U
#define MAX_RESOLUTION		12U
#define VREF_ADC			3.3F
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Public functions -----------------------------------------------------------*/
bool_t 				adcInit(ADC_HandleTypeDef *ADCHandle, uint8_t bits,UART_HandleTypeDef *UARTHandle);
uint16_t 			adcConversion(ADC_HandleTypeDef *ADCHandle);
float				adcVoltage(ADC_HandleTypeDef *ADCHandle);

#endif /* API_INC_API_ADC_H_ */
