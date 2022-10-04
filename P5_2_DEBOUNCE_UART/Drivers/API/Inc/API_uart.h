/*
 * API_uart.h
 *
 *  Created on: 4 oct. 2022
 *      Author: Yonatan Aguirre
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx_hal.h"

typedef bool bool_t;

#define SUCCESSFUL 		true
#define UNSUCCESSFUL 	false

#define DEFAULT_BAUD	9600
#define MAX_BAUD		115200	//Maximum baud rate
#define TIMEOUT			5000

#define MAX_LEN_SMS		30		//Maximum size for custom messages at UART initialization

bool_t uartInit(UART_HandleTypeDef *UartHandle,uint32_t baud);
void uartSendString(UART_HandleTypeDef *UartHandle, uint8_t * pstring);
void uartSendStringSize(UART_HandleTypeDef *UartHandle,uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(UART_HandleTypeDef *UartHandle,uint8_t * pstring, uint16_t size);

#endif /* API_INC_API_UART_H_ */
