/* USER CODE BEGIN Header */
/**
  *****************************************************************************************
  * @file           : API_adc.c
  * @brief          : API for the configuration and reading of an analog channel of an ADC
  * 				  The implemented functions are:
  *  			 		1.initSuccessful
  *  			 		2.adcInit
  *  			 		3.adcConversion
  *  			 		4.adcVoltage
  *                   This file contains the common defines of the application.
  * Created on		: Oct 10, 2022
  *  Author			: Yonatan Aguirre
  *****************************************************************************************
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

#include "API_adc.h"

/* Private define -------------------------------------------------------------*/
/* Number to voltage conversion factors
 * FACT_CONV_N2V = Vref/(2^n - 1)
 * n	: bit resolution
 * Vref : internal voltage reference (3.3V)
 * */
#define FACT_CONV_N2V_12B	0.000805664F
#define FACT_CONV_N2V_10B	0.003225806F
#define FACT_CONV_N2V_8B	0.012941176F

/* Private variables -------------------------------------------------------------*/
static uint16_t adc_value;
static float voltage;
static float factorN2V;
static uint16_t maxValueADC;

UART_HandleTypeDef *UartHandleAPI;

/* Private functions -----------------------------------------------------------*/
static void initSuccessful(uint8_t bits);

/**
 * @brief:  Prints the current ADC configuration parameters
 * @param1: UartHandle pointer to the UART_Handle structure
 * @retval: none
 */
static void initSuccessful(uint8_t bits)
{
	/**
	 * Initialize the sms vector with the memset function and then use strcpy to copy the custom string
	 * for each parameter except for the resolution, where we use sprintf to format the message
	 * with the numerical value of the bits. The function uartSendString sends the messages over the UART.
	 */

	char smsPar[MAX_LEN_SMS];

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar,"*******ADC parameters********\r\n");
	uartSendString(UartHandleAPI,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	sprintf(smsPar," Resolution: %2u\r\n",bits);
	uartSendString(UartHandleAPI,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar," Channel: 1\r\n");
	uartSendString(UartHandleAPI,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar," Clock: 30MHZ\r\n");
	uartSendString(UartHandleAPI,(uint8_t *)smsPar);

	memset(smsPar,'\0',sizeof(smsPar));
	strcat(smsPar,"****************************\r\n");
	uartSendString(UartHandleAPI,(uint8_t *)smsPar);
}

/**
 * @brief:  Initializes the ADC Module and prints the configuration parameters of the module itself
 * 			For our application, we only select the analog channel 1
 * @param1: ADCHandle pointer to the ADC_HandleTypeDef structure
 * @param2: bit resolution of converter
 * @param3: UartHandle pointer to the UART_HandleTypeDef structure
 * @retval: Boolean value. If the module initializes of successful form: true, otherwise: false
 */
bool_t adcInit(ADC_HandleTypeDef *ADCHandle, uint8_t bits,UART_HandleTypeDef *UARTHandle)
{
	bool_t configStatus=UNSUCCESSFUL;

	ADC_ChannelConfTypeDef sConfig = {0};

	if (UARTHandle != NULL && ADCHandle != NULL)
	{
		UartHandleAPI = UARTHandle;

		/***************************************************************************************************
		 * Configure the global features of the ADC:
		 *  1.Clock
		 *  2.Resolution
		 *  3.Data Alignment
		 *  4.Number of conversion)
		 *  Note: The maximum ADC conversion clock frequency is 36MHz (page 113 - STM32F411 datasheet).
		 *  Since the peripheral clock is 60MHz, that is why a divider of value 2 is selected.
		 **************************************************************************************************/
		ADCHandle->Instance = ADC1;
		ADCHandle->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;

		switch(bits)
		{
		case 8:
			ADCHandle->Init.Resolution = ADC_RESOLUTION_8B;
			factorN2V = FACT_CONV_N2V_8B;
			maxValueADC = MAX_VALUE_8B;
			break;

		case 10:
			ADCHandle->Init.Resolution = ADC_RESOLUTION_10B;
			factorN2V = FACT_CONV_N2V_10B;
			maxValueADC = MAX_VALUE_10B;
			break;

		case 12:
			ADCHandle->Init.Resolution = ADC_RESOLUTION_12B;
			factorN2V = FACT_CONV_N2V_12B;
			maxValueADC = MAX_VALUE_12B;
			break;

		default:
			bits = MAX_RESOLUTION;
			ADCHandle->Init.Resolution = ADC_RESOLUTION_12B;
			factorN2V = FACT_CONV_N2V_12B;
			maxValueADC = MAX_VALUE_12B;
			break;
		}

		ADCHandle->Init.ScanConvMode = DISABLE;
		ADCHandle->Init.ContinuousConvMode = DISABLE;
		ADCHandle->Init.DiscontinuousConvMode = DISABLE;
		ADCHandle->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		ADCHandle->Init.ExternalTrigConv = ADC_SOFTWARE_START;
		ADCHandle->Init.DataAlign = ADC_DATAALIGN_RIGHT;
		ADCHandle->Init.NbrOfConversion = 1;
		ADCHandle->Init.DMAContinuousRequests = DISABLE;
		ADCHandle->Init.EOCSelection = ADC_EOC_SINGLE_CONV;

		/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
		 */
		sConfig.Channel = ADC_CHANNEL_1;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

		if ((HAL_ADC_Init(ADCHandle) == HAL_OK) && (HAL_ADC_ConfigChannel(ADCHandle, &sConfig) == HAL_OK))
		{	configStatus = SUCCESSFUL;
		initSuccessful(bits);	//call routine to print the configuration parameters
		}
		else
		{
			configStatus = UNSUCCESSFUL;
			uartSendString(UartHandleAPI,(uint8_t *)"Failed to initialize ADC module\r\n");
		}

	}

	return configStatus;
}

/**
 * @brief:  Performs reading of analog channel by polling method
 * @param1: ADCHandle pointer to the ADC_HandleTypeDef structure
 * @retval: 2-byte unsigned integer value corresponding to the conversion value on the selected channel
 */
uint16_t adcConversion(ADC_HandleTypeDef *ADCHandle)
{
	if(ADCHandle != NULL)
	{
		HAL_ADC_Start(ADCHandle);
		HAL_ADC_PollForConversion(ADCHandle, HAL_MAX_DELAY);
		adc_value = HAL_ADC_GetValue(ADCHandle);

		/*Validate that the conversion value is within the allowed range according to the selected resolution.*/
		if (adc_value > maxValueADC)
			adc_value = maxValueADC;
	}
	else
		adc_value = 0;

	return adc_value;
}

/**
 * @brief:  Calculates the value of the input voltage at the analog channel
 * @param1: ADCHandle pointer to the ADC_HandleTypeDef structure
 * @retval: Float value, corresponding to input voltage
 */
float adcVoltage(ADC_HandleTypeDef *ADCHandle)
{
	uint16_t value;

	if (ADCHandle != NULL)
	{
		value = adcConversion(ADCHandle);

		/* Validation of both the ADC return value and the output voltage value returned to the user.
		 * In both cases a limitation is made according to the maximum allowed values.
		 * */
		if (value > maxValueADC)
			value = maxValueADC;

		voltage = (float)adc_value*factorN2V;

		if (voltage>VREF_ADC)
			voltage = VREF_ADC;
	}
	else
		voltage = 0.0;

	return voltage;
}
