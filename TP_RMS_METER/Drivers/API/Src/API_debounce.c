/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : API_debounce.c
  * @brief          : API for reading the current state of the user button, through the implementation of a FSM.
  * 				  The implemented functions are:
  *  			 		1.debounceFSM_init
  *  			 		2.debounceFSM_update
  *  			 		3.buttonPressed
  *  			 		4.readKey
  *  			 		5.fallingEdge
  *  			 		6.raisingEdge
  *  			 		7.errorFSM
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

#include "API_debounce.h"

/* Private types ---------------------------------------------------*/
/* States for FSM*/
typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

static debounceState_t fsmButtonState;
delay_t delayNbLED;
/* Global Private Variables ----------------------------------------*/
static bool_t keyPressed = false;	//save status of key
static bool_t flagFallingEdge = false;
static bool_t flagRaisingEdge = false;

/* Private functions -----------------------------------------------------------*/
static void buttonPressed();
static void errorFSM();

/**
  * @brief  Initializes the FSM with the initial state of the button and delay for anti-rebound
  * @param  None
  * @retval None
  */
void debounceFSM_init()
{
	fsmButtonState = BUTTON_UP;				//initial state FSM
	delayInit(&delayNbLED,DEBOUNCE_DELAY);	//initialize structure to implement a delay of 40ms
}


/**
  * @brief:  Reads the state of the user button and implements state transition logic to update the outputs
  * 		 by triggering events buttonPressed y buttonReleased
  * @param:  None
  * @retval: None
  */
void debounceFSM_update()
{
	switch (fsmButtonState)
	{
		case BUTTON_UP:	//initial state
			if(!BSP_PB_GetState(BUTTON_USER))
			{
				fsmButtonState = BUTTON_FALLING;	//falling edge detected
				delayRead(&delayNbLED);				//initialize delay
			}
			break;

		case BUTTON_FALLING:
			if(delayRead(&delayNbLED))	// check if the anti-rebound delay has expired
			{
				if(!BSP_PB_GetState(BUTTON_USER))	//new reading to check if the button was pressed
				{
					fsmButtonState = BUTTON_DOWN;
					flagFallingEdge = true;
					buttonPressed();	//valid button press -> call to button pressed event
				}
				else
					fsmButtonState = BUTTON_UP;	// button is not pressed, return to initial state
			}
			break;

		case BUTTON_DOWN:
			if(BSP_PB_GetState(BUTTON_USER))
			{
				fsmButtonState = BUTTON_RAISING;	//rising edge detected
				delayRead(&delayNbLED);				//initialize delay
			}
			break;

		case BUTTON_RAISING:
			if(delayRead(&delayNbLED))	// check if the anti-rebound delay has expired
			{
				if(BSP_PB_GetState(BUTTON_USER))	//new reading to check if the button is not pressed
				{
					fsmButtonState = BUTTON_UP;
					flagRaisingEdge = true;
				}
				else
					fsmButtonState = BUTTON_DOWN; // button still pressed, return to button down state
			}
			break;

		default:
			errorFSM();
			break;
	}
}

/**
  * @brief  Event generated when validating that the button is pressed.
  * @param  None
  * @retval None
  */
void buttonPressed()
{
	keyPressed = true;
}

/**
  * @brief  Function for return state of Key
  * @param  None
  * @retval True: If key is pressed or False: If key is not pressed
  */
bool_t readKey()
{
	bool_t keyPressedAux = false;

	if(keyPressed)
	{
		keyPressedAux = true;
		keyPressed = false;
	}

	return keyPressedAux;
}

/**
  * @brief  Function to check if a valid falling edge was detected.
  * @param  None
  * @retval True: If a valid falling edge was detected or False in otherwise
  */
bool_t fallingEdge()
{
	bool_t fallingEdgeStatus = false;

	if(flagFallingEdge)
	{
		fallingEdgeStatus = true;
		flagFallingEdge = false;
	}

	return fallingEdgeStatus;
}

/**
  * @brief  Function to check if a valiUSERd raising edge was detected.
  * @param  None
  * @retval True: If a valid raising edge was detected or False in otherwise
  */
bool_t raisingEdge()
{
	bool_t raisingEdgeStatus = false;

	if(flagRaisingEdge)
	{
		raisingEdgeStatus = true;
		flagRaisingEdge = false;
	}

	return raisingEdgeStatus;
}


/**
  * @brief  Function for turn on LED2 if an error occurs in FSM
  * @param  None
  * @retval NOne
  */
void errorFSM()
{
	BSP_LED_On(LED2);
}

