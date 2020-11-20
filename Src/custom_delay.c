/*
 * custom_delay.c
 *
 *  Created on: 19.07.2020
 *      Author: Dirk
 */

#include "custom_delay.h"
#include "tim.h"

void delay_ms(uint16_t delay)
{
	/* Start Counter */
	if (HAL_TIM_Base_Start(&htim2) != HAL_OK)
	{
	    Error_Handler();
	}

	/* Reset counter to zero */
	__HAL_TIM_SET_COUNTER(&htim2, 0);

	/* Wait until counter reaches delay value */
	while( __HAL_TIM_GET_COUNTER(&htim2) < delay)
		{
			/* do nothing */
		}

	/* Stop timer */
	HAL_TIM_Base_Stop(&htim2);

}
