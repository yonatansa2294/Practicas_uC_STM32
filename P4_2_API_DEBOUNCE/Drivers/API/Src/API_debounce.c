/*
 * API_debounce.c
 *
 *  Created on: 18 set. 2022
 *      Author: Yonatan Aguirre
 */

/**
  * @brief  Initilizes the FSM with the initial state of the unpressed button
  * @param  None
  * @retval None
  */
#include "API_debounce.h"

/*-------------------- Global Private Variables ---------------------------*/
/* Enumeration of states for FSM*/
typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

static debounceState_t fsmButtonState;
static bool_t keyPressed = false;	//save status of key
delay_t delayNbLED;				//structure for delay generation

/*------ Private Functions Prototype------*/
static void buttonPressed();
static void buttonReleased();
static void errorFSM();

/**
  * @brief  Initializes the FSM with the initial state of the button and delay for anti-rebound
  * @param  None
  * @retval None
  */
void debounceFSM_init()
{
	fsmButtonState = BUTTON_UP;
	delayInit(&delayNbLED,DEBOUNCE_DELAY);	//initialize structure to implement a delay of 40ms
	BSP_LED_Init(LED_USER);
}


/**
  * @brief  Reads the state of the user button and implements state transition logic to update the outputs
  * by triggering events buttonPressed y buttonReleased
  * @param  None
  * @retval None
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
					buttonReleased();	//valid button released -> call to button released event
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
  * @brief  Event generated when validating that the button is no longer pressed.
  * @param  None
  * @retval None
  */
void buttonReleased()
{

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
  * @brief  Function for turn on LED_USER if an error occurs in FSM
  * @param  None
  * @retval NOne
  */
void errorFSM()
{
	BSP_LED_On(LED_USER);
}

