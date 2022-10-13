/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_rms.c
  * @brief          : API for the calculation of the RMS current value.
  * 				  The implemented functions are:
  *  			 		1.getSamples
  *  			 		2.rmsFSM_init
  *  			 		3.rmsFSM_update
  *  			 		4.readRMSCurrent
  *                   This file contains the common defines of the application.
  * Created on		: Oct 10, 2022
  * Author			: Yonatan Aguirre
  * Note			: For more information about the implemented measurement algorithm, please consult
  *     			  https://ebooks.iospress.nl/volumearticle/58780
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

#include "API_rms.h"

/**********************************************************************************************************************
 * Description of constants used for RMS value calculation
 * Alpha 			: Current sensor conversion factor -> max. input current: 100A - max. output current: 50mA
 * Beta	 			: ADC factor -> Maximum numerical value / Vref. The maximum numerical value depends
 * 		   			  on the bit resolution selected for the converter. For this case it is 12 bits
 * GF	 			: Sallen Key filter gain at 60Hz, present in the signal conditioning stage.
 * RS	 			: Resistance value placed at the current sensor to obtain a maximum voltage drop of 1VRMS
 * FACTOR_SIG_COND 	: Factor used to calculate the current value measured by the sensor from the calculated rms value.
 * SAMPLES			: Number of samples required by the algorithm
 *
 * The input to the conditioning stage is a 1VRMS signal from the sensor connected to the RS resistor.
 * The signal conditioning stage consists of a second order Sallen Key filter with a cutoff frequency of Fc=159Hz,
 * and an additional circuit to add an offset of 1.66V to shift the signal to obtain a signal with positive values
 * between 0.4v and 2.9v, taking advantage of almost the entire range of the ADC available for conversion.
 **********************************************************************************************************************/
#define ALPHA					0.0005F
#define BETA					1240.909F
#define GF						0.8756F
#define RS						20.0F
#define FACTOR_SIG_COND			(1/(ALPHA*BETA*GF*RS))
#define SAMPLES					4U
#define LENGTH_SMS				40U

/* Private types ------------------------------------------------------------*/
/* States for FSM*/
typedef enum
{
	WAIT_FOR_NEW_SC=0,
	COMPUTE_RMS,
	RESTART
} rmsMeterFSMState_t;

static rmsMeterFSMState_t meterFSMState;

ADC_HandleTypeDef *ADCHandleAPI;
TIM_HandleTypeDef *TMRHandleAPI;
UART_HandleTypeDef *UARTHandleAPI;
/* Private variables ------------------------------------------------------------*/
volatile static uint16_t ik[SAMPLES];	//current sample vector
volatile static uint8_t k = 0;			//sample counter
volatile static int16_t sal,cal;
static int16_t sal_temp,cal_temp;
volatile static bool_t flag_new_SC;
static float rmsValue;

/**
 * @brief:  Routine for sample capture and calculation of SAL and CAL filters
 * 			This routine is called every 4.16ms when the timer overflow interrupt occurs.
 * @retval: none
 */
void getSamples()
{
	/* Prevent unused argument(s) compilation warning */
	BSP_TICK_On();	//interrupt entry

	ik[k] = adcConversion(ADCHandleAPI);	//get sample
	k++;	//increment counter

	/* We check if we have 4 samples to calculate SAL and CAL */
	if(SAMPLES == k)
	{

		/*********************************************************************************************************
		 * SAL and CAL filters
		 * The coefficients of these filters  are defined by the second and third one-dimensional Walsh function
		 * and are used to measure the phasor of the RMS current signal
		 *********************************************************************************************************/
		sal = ik[3] + ik[2] - ik[1] - ik[0];
		cal = ik[3] - ik[2] - ik[1] + ik[0];
		flag_new_SC = true;	//set the flag to indicate if there is new SAL and CAL data
		k = 0;	//reset counter
	}

	BSP_TICK_Off();	//interrupt exit
}

/**
 * @brief:  Initializes the FSM for the acquisition and calculation of the RMS value
 * 			as well as triggers the timer overflow interrupt.
 * @param1: UartHandle pointer to the UART_Handle structure
 * @param2: ADCHandle pointer to the ADC_HandleTypeDef structure
 * @param3: UARTHandle pointer to the UART_HandleTypeDef structure
 * @retval: None
 */
bool_t rmsFSM_init(TIM_HandleTypeDef *TMRHandle, ADC_HandleTypeDef *ADCHandle,  UART_HandleTypeDef *UARTHandle)
{
	bool_t init=UNSUCCESSFUL;

	if (UARTHandle != NULL && ADCHandle != NULL && TMRHandle != NULL)
	{
		meterFSMState = WAIT_FOR_NEW_SC;	//initial state of FSM
		flag_new_SC = false;
		TMRHandleAPI = TMRHandle;
		ADCHandleAPI = ADCHandle;
		UARTHandleAPI = UARTHandle;
		timerStart(TMRHandle);	//start of interruption
		init = SUCCESSFUL;
	}
	else
		init = UNSUCCESSFUL;

	return init;
}

/**
  * @brief:  Implements the state transition logic of the FSM to calculate the RMS current phasor and
  * 		 and the value of the RMS voltage signal at the input of the analog channel.
  * @param:  None
  * @retval: None
  */
void rmsFSM_update()
{

	switch(meterFSMState)
	{
		case WAIT_FOR_NEW_SC:
			if(flag_new_SC == true)
				meterFSMState = COMPUTE_RMS;

		case COMPUTE_RMS:
			/*Save copies of the current SAL and CAL values to perform the calculations*/
			sal_temp = sal;
			cal_temp = cal;
			/************************************************************************
			 * The calculation of the RMS value is expressed by the relation:
			 * Irms = |phasor| = (SAL² + CAL²)^(1/2)
			 *        --------   -------------------
			 *           4               4
			 *
			 * It should be noted that the accuracy of the measurement depends on maintaining
			 * the relation Fs = 4FL,
			 * Where:
			 * Fs : sampling frequency
			 * FL : line frequency
			 **/
			rmsValue= sqrt(sal_temp*sal_temp + cal_temp*cal_temp)/4;
			meterFSMState = RESTART;
			break;

		case RESTART:
			flag_new_SC = false;
			meterFSMState = WAIT_FOR_NEW_SC;
			break;
	}

}

/**
 * @brief:  Calculates the RMS current value measured by the sensor
 * @retval: Float value of RMS current
 */
float readRMSCurrent()
{
	float irmsValue,irmTemp;

	irmTemp = rmsValue;
	irmsValue = irmTemp*FACTOR_SIG_COND;

	return irmsValue;

}
