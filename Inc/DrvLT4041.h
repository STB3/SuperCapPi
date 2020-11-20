/*
 * DrvLT4041.h
 *
 *  Created on: 01.06.2020
 *      Author: Dirk
 */

#ifndef DRVLT4041_H_
#define DRVLT4041_H_

#include "main.h"

#define SHUTDOWN_DELAY_STEP			100 //milliseconds
#define SHUTDOWN_TIMEOUT			200 // x SHUTDOWN_DELAY_STEP milliseconds

#define UPS_BACKUP_TIME				2000 // milliseconds
#define SHUTDOWN_ACK_PULSE_LEN		1000 // milliseconds
#define POWER_ON_HOLD				2000 // milliseconds
#define REPOWER_DELAY				2000 // milliseconds
#define BUTTON_DEBOUNCE_INTERVAL  	  20 // milliseconds

uint32_t state, prev_state;

void EnterStopMode(void);
void RunMode(void);

#endif /* DRVLT4041_H_ */
