/*
 * DrvLT4041.c
 *
 *  Created on: 01.06.2020
 *      Author: Dirk
 */

#include "gpio.h"
#include "DrvLT4041.h"
#include "DrvEEPROM.h"
#include "custom_delay.h"

/* µController mode switching */
void EnterStopMode(void)
{
    /* Disable debug in stop mode to reduce powerconsumption */

#ifdef DEBUG
	HAL_DBGMCU_DisableDBGStopMode();
#else
  HAL_DBGMCU_EnableDBGStopMode();
#endif
	/* Put I/Os in optimal power save conditions */
	UnusedGPIO_StopMode();

	/* Clear Wake Up Flag */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	/* Enter Stop Mode */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    /* Configures system clock after wake-up from STOP */
    SystemClock_Config();
    /* Restore pin state */
    UnusedGPIO_RunMode();
}


/* Shutting down the raspberry pi power supply 		*/
/* depending on if wall plug is connected or not 	*/
void Shutdown(void)
{
	int32_t timeout;

	timeout = SHUTDOWN_TIMEOUT;
	delay_ms(100);
	LOWSIDE_SW_OPEN();
	delay_ms(100);
	POWER_GOOD_2RPI_OFF();

	while (timeout > 0)
	{
		timeout--;

		if(timeout % 5 == 0) STATUS_LED_TOGGLE(); // toggle LED each SHUTDOWN_DELAY_STEP * 5

		delay_ms(SHUTDOWN_DELAY_STEP);
	}

	CHARGER_BOOSTER_OFF();
	STATUS_LED_OFF();
}

/* Power up raspberry pi */
void RunMode(void)
{
	uint8_t i;
	STATUS_LED_ON();
	for(i=0;i<5;i++)
	{
		CHARGER_BOOSTER_ON();
		delay_ms(5);
		CHARGER_BOOSTER_OFF();

		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");

		CHARGER_BOOSTER_ON();
	}
    delay_ms(POWER_ON_HOLD);
	POWER_GOOD_2RPI_ON();
	delay_ms(200);
	LOWSIDE_SW_CLOSED();
}


/* Statemachine handling the different states the board can have */
void HAL_GPIO_EXTI_Callback(volatile uint16_t GPIO_Pin)
{
	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn); 	// Ignore spurious trigger caused by "Shutdown();"
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);	// Ignore trigger caused by rasPI application SW

	prev_state = load_eePROMqueue();	 /* In case of a total power loss.*/
	state = prev_state;

	/* Check wich pin was triggered */
	switch (GPIO_Pin)
	{
		case BUTTON_Pin:
		delay_ms(BUTTON_DEBOUNCE_INTERVAL);
		if(GET_BUTTON() == button_state)
			{
			if (GET_POWER_GOOD() == 1) // Means USB plug is connected and unit was shut down by button
			{
				if(prev_state == OFF_powered || prev_state == OFF_unpowered)
				{
					// Previous state must be OFF_powered
					state = ON_powered;
					RunMode();
				}
				else if (prev_state == ON_powered || prev_state == ON_unpowered)
				{

					Shutdown();

					if (GET_POWER_GOOD() == 1)	// Check if USB was disconnected during shutdown
					{
					state = OFF_powered;
					}
					else
						{
							state = OFF_unpowered;
						}
				}
				else if (prev_state == APPshtdn_powered || prev_state == APPshtdn_unpowered)
				{
					state = ON_powered;
					RunMode();
				}
				else
				{
					// Should never go here
					state = ON_powered;
					RunMode();
				}
			}
			else
			{
				// Do nothing because unit is not powered
				state = prev_state;
			}
		}
			break;

		case PWR_GOOD_Pin:
			delay_ms(UPS_BACKUP_TIME); 							// Wait to stabilize power good signal & timeout for UPS function (below xx ms the power loss is ignored)
			if(GET_POWER_GOOD() == power_good_state) 	// Power good signal stable otherwise do nothing
			{
				if (GET_POWER_GOOD() == 1)	/* Power good wakes up at both edges, need to find out which edge triggered */
				{
					if(prev_state == OFF_unpowered || prev_state == OFF_powered )
					{
						state = OFF_powered;
						// No changes in signals
					}
					else if (prev_state == ON_powered || prev_state == ON_unpowered)
					{
						state = ON_powered;
						RunMode();
					}
					else if (prev_state == APPshtdn_powered || prev_state == APPshtdn_unpowered)
					{
						state = ON_powered;
						RunMode();
					}
					else
					{
						// Should never go here
						state = ON_powered;
						RunMode();
					}
				}
				else if (GET_POWER_GOOD() == 0)
				{
					if(prev_state == ON_powered)
					{
						Shutdown();

						if (GET_POWER_GOOD() == 1)	// Check if USB was reconnected during shutdown
						{
							state = ON_powered;
							delay_ms(REPOWER_DELAY); 		// Leave PI off for some time
							RunMode();
						}
						else
							{
								state = ON_unpowered;
							}
					}
					else if(prev_state == OFF_powered)
					{
						state = OFF_unpowered;
						// No changes in signals

					}
					else if(prev_state == APPshtdn_powered || prev_state == APPshtdn_unpowered)
					{
						state = APPshtdn_unpowered;
						// No changes in signals

					}
					else
					{
						// Should never go here
						state = OFF_unpowered;
					}
				}
				else
				{
					// Should never go here
				}
			}
			break;

		case SHTDN_ACK_Pin:
			delay_ms((uint16_t)(SHUTDOWN_ACK_PULSE_LEN / 2));	// Wait until 50% of the shutdown pulse is reached
			if(GET_SHUTDOWN_ACK() == shutdown_ack_state) 		// Check if signal was stable (c-stop holds pin 1sec high)
			{
				if (GET_POWER_GOOD() == 1 && prev_state == ON_powered) // Means USB plug is connected and unit was requested to shut down by application
				{

					Shutdown();

					if (GET_POWER_GOOD() == 1)	// Check if USB was disconnected during shutdown
					{
						state = APPshtdn_powered;
					}
					else
						{
							state = APPshtdn_unpowered;
						}

				}
				else
				{
					// do nothing
				}
			}
			else
			{
				// do nothing
			}
			break;

		default:
			break;
	}

	if (prev_state != state)	// Store new state in case it different from previous state
		{
			write_eePROMvalue(state);
		}

	__HAL_GPIO_EXTI_CLEAR_IT(PWR_GOOD_Pin);		// Clear pending interrupts from LTC4041,
	__HAL_GPIO_EXTI_CLEAR_IT(BUTTON_Pin);		// button
	__HAL_GPIO_EXTI_CLEAR_IT(SHTDN_ACK_Pin);	// and shutdown acknowledge signal from rPI

	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);			// (re-)enable button and LTC4041 power fail IRQ
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);			// (re-)enable shutdown acknowledge from rasPI
}

