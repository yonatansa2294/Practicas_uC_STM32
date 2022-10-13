/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_uart.c
  * @brief          : API to modularize the sending and receiving of data through the UART.
  * 				  The implemented functions are:
  *  			 		1.initSuccessful
  *  			 		2.uartInit
  *  			 		3.uartSendString
  *  			 		4.uartSendStringSize
  *  			 		5.uartReceiveStringSize
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

#include "API_uart.h"
/* Private variables -----------------------------------------------------------*/
static char errorSMS[MAX_LEN_SMS]="Error - Invalid parameters\r\n";
/* Private functions -----------------------------------------------------------*/
static void initSuccessful(UART_HandleTypeDef *UartHandle,uint32_t baud);

/**
 * @brief:  Prints the current UART configuration parameters
 * @param1: UartHandle pointer to the UART_Handle structure
 * @param2: Baud rate.
 * @retval: none
 */
static void initSuccessful(UART_HandleTypeDef *UartHandle,uint32_t baud)
{
	/* Initialize the sms vector with the memset function and then use strcpy to copy the custom string
	 * for each parameter except for the baud rate, where we use sprintf to format the message with the numerical value
	 * of the baud rate. The function uartSendString sends the messages over the UART.*/

	char smsPar[MAX_LEN_SMS];

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar,"*******UART parameters******\r\n");
	uartSendString(UartHandle,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar," UART: 1\r\n");
	uartSendString(UartHandle,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	sprintf(smsPar," BaudRate: %5lu\r\n",baud);
	uartSendString(UartHandle,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar," Bits: 8\r\n");
	uartSendString(UartHandle,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar," Stop Bits: 1\r\n");
	uartSendString(UartHandle,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar," Parity: Odd\r\n");
	uartSendString(UartHandle,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar," Flow control: none\r\n");
	uartSendString(UartHandle,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar,"****************************\r\n");
	uartSendString(UartHandle,(uint8_t *)smsPar);
}

/**
 * @brief:  Initializes the UART Module and prints the configuration parameters of the module itself
 * @param1: UartHandle pointer to the UART_Handle structure
 * @param2: Baud rate. Maximum baud rate is 115200 and default baud rate is 9600.
 * @retval: Boolean value. If the module initializes of successful form: true, otherwise: false
 */
bool_t uartInit(UART_HandleTypeDef *UartHandle,uint32_t baud)
{
	bool_t init=UNSUCCESSFUL;

	if(UartHandle != NULL && baud != 0)
	{
		/**************************** Configure the UART peripheral **********************************
			 	Put the USART peripheral in the Asynchronous mode (UART Mode)
			 	UART configured as follows:
			 	  - Word Length = 8 Bits (7 data bit + 1 parity bit) :
			 					  BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
			 	  - Stop Bit    = One Stop bit
			 	  - Parity      = ODD parity
			 	  - BaudRate    = 9600 baud for default.
			 	  - Hardware flow control disabled (RTS and CTS signals)
		 ********************************************************************************************/
		UartHandle->Instance = USART1;

		/* Check if a baud rate within the allowed values has been entered.
		 * If the maximum baud rate is exceeded, the default is 9600 baud.*/
		if(baud>MAX_BAUD)
			UartHandle->Init.BaudRate = DEFAULT_BAUD;
		else
			UartHandle->Init.BaudRate = baud;

		UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
		UartHandle->Init.StopBits = UART_STOPBITS_1;
		UartHandle->Init.Parity = UART_PARITY_ODD;

		UartHandle->Init.Mode = UART_MODE_TX_RX;
		UartHandle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UartHandle->Init.OverSampling = UART_OVERSAMPLING_16;

		if (HAL_UART_Init(UartHandle) == HAL_OK)
		{
			init = SUCCESSFUL;
			initSuccessful(UartHandle,baud);	//call routine to print the configuration parameters
		}
		else
			init = UNSUCCESSFUL;	//initialization error
	}

	return init;
}

/**
 * @brief:  Sends a full string of data over the specified UART module
 * @param1: UartHandle pointer to the UART_Handle structure indicating the UART to be used
 * @param2: pointer to the data string to be sent
 * @retval: None
 */
void uartSendString(UART_HandleTypeDef *UartHandle, uint8_t * pData)
{
	if(UartHandle != NULL && pData!= NULL)
	{
		uint16_t dataLength = strlen((const char*)pData); //number of characters in the string.
		HAL_UART_Transmit(UartHandle, (uint8_t *)pData, dataLength, TIMEOUT);
	}
	else
		HAL_UART_Transmit(UartHandle, (uint8_t *)errorSMS,strlen((const char*)errorSMS), TIMEOUT);
}

/**
 * @brief:  Sends a specific number of characters of the string over the specified UART module
 * @param1: UartHandle pointer to the UART_Handle structure indicating the UART to be used
 * @param2: pointer to the data string to be sent
 * @param2: number of characters to send
 * @retval: None
 */
void uartSendStringSize(UART_HandleTypeDef *UartHandle,uint8_t * pData, uint16_t size)
{
	if(size > 0 && UartHandle != NULL && pData != NULL)
		HAL_UART_Transmit(UartHandle, (uint8_t *)pData, size, TIMEOUT);
	else
		HAL_UART_Transmit(UartHandle, (uint8_t *)errorSMS,strlen((const char*)errorSMS), TIMEOUT);
}

/**
 * @brief:  Receives a string over the UART with a specific size defined by the user over the specified UART module
 * @param1: UartHandle pointer to the UART_Handle structure indicating the UART to be used
 * @param2: Pointer to a data string where the received information is stored.
 * @param2: Positive integer number of characters to receive
 * @retval: None
 */
void uartReceiveStringSize(UART_HandleTypeDef *UartHandle,uint8_t * dataRX, uint16_t size)
{
	if(size > 0 && UartHandle != NULL && dataRX != NULL)
		HAL_UART_Receive(UartHandle, (uint8_t *)dataRX, size, TIMEOUT);
}
