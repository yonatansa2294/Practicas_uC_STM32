/*
 * API_debounce.h
 *
 *  Created on: 18 set. 2022
 *      Author: Yonatan Aguirre
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "main.h"
#include "API_delay.h"

#define DEBOUNCE_DELAY	40U	//delay for debounce in [ms]

/*------ Prototype Publics Functions */
void debounceFSM_init();
void debounceFSM_update();
bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */
