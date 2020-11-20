/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */


#define CHARGER_BOOSTER_ON()	HAL_GPIO_WritePin(NCHARGER_NBOOSTER_ENB_GPIO_Port, NCHARGER_NBOOSTER_ENB_Pin, GPIO_PIN_RESET)
#define CHARGER_BOOSTER_OFF()	HAL_GPIO_WritePin(NCHARGER_NBOOSTER_ENB_GPIO_Port, NCHARGER_NBOOSTER_ENB_Pin, GPIO_PIN_SET)

#define POWER_GOOD_2RPI_ON()	HAL_GPIO_WritePin(POWER_GOOD2RPI_GPIO_Port, POWER_GOOD2RPI_Pin, GPIO_PIN_SET)
#define POWER_GOOD_2RPI_OFF()	HAL_GPIO_WritePin(POWER_GOOD2RPI_GPIO_Port, POWER_GOOD2RPI_Pin, GPIO_PIN_RESET)

#define GET_POWER_GOOD()		HAL_GPIO_ReadPin(PWR_GOOD_GPIO_Port, PWR_GOOD_Pin)

#define GET_CAP_CHARGED()		HAL_GPIO_ReadPin(CAP_CHARGED_GPIO_Port, CAP_CHARGED_Pin)

#define GET_SHUTDOWN_ACK()		HAL_GPIO_ReadPin(SHTDN_ACK_GPIO_Port, SHTDN_ACK_Pin)

#define GET_BUTTON()			HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin)
#define STATUS_LED_TOGGLE()		HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin)
#define STATUS_LED_OFF()		HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET)
#define STATUS_LED_ON() 		HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET)

#define LOWSIDE_SW_CLOSED()		HAL_GPIO_WritePin(LOWSIDE_SW_GPIO_Port, LOWSIDE_SW_Pin, GPIO_PIN_SET)
#define LOWSIDE_SW_OPEN()		HAL_GPIO_WritePin(LOWSIDE_SW_GPIO_Port, LOWSIDE_SW_Pin, GPIO_PIN_RESET)

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void enablePullUp(GPIO_TypeDef  *GPIOx, uint32_t pin);
void enablePullDown(GPIO_TypeDef  *GPIOx, uint32_t pin);
void disablePullResitor(GPIO_TypeDef  *GPIOx, uint32_t pin);

void UnusedGPIO_RunMode(void);
void UnusedGPIO_StopMode(void);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
