/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_uart.h
  * @brief          : Header for API_uart.c file.
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
#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
/* Private includes ----------------------------------------------------------*/
#include "stm32f4xx_hal.h"		/* <- HAL include */
/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;
/* Public define -------------------------------------------------------------*/
#define SUCCESSFUL 		true
#define UNSUCCESSFUL 	false
#define DEFAULT_BAUD	9600LU
#define MAX_BAUD		115200LU
#define TIMEOUT			5000LU
#define MAX_LEN_SMS		30U		//Maximum size for custom messages at UART initialization
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Public functions -----------------------------------------------------------*/
bool_t 		uartInit(UART_HandleTypeDef *UartHandle,uint32_t baud);
void 		uartSendString(UART_HandleTypeDef *UartHandle, uint8_t * pstring);
void 		uartSendStringSize(UART_HandleTypeDef *UartHandle,uint8_t * pstring, uint16_t size);
void 		uartReceiveStringSize(UART_HandleTypeDef *UartHandle,uint8_t * pstring, uint16_t size);

#endif /* API_INC_API_UART_H_ */
