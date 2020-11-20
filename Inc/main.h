/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

enum states
{
	state_undefined = 0,
	ON_powered,				// Unit is running
	OFF_powered,			// Shutdown was initiated by pushing the power button
	APPshtdn_powered,		// Shutdown was initiated by the application SW
	APPshtdn_unpowered,		// Shutdown was initiated by the application SW and later the supply was disconnected
	ON_unpowered,			// Unit was running when the supply was disconnected
	OFF_unpowered			// Unit was off when the supply was disconnected
};


uint8_t power_good_state;
uint8_t shutdown_ack_state;
uint8_t button_state;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

void SystemClock_Config(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SHTDN_ACK_Pin GPIO_PIN_10
#define SHTDN_ACK_GPIO_Port GPIOA
#define POWER_GOOD2RPI_Pin GPIO_PIN_14
#define POWER_GOOD2RPI_GPIO_Port GPIOC
#define LOWSIDE_SW_Pin GPIO_PIN_15
#define LOWSIDE_SW_GPIO_Port GPIOC
#define LOWSIDE_SW_ENB_GPIO_Port GPIOC
#define BUTTON_Pin GPIO_PIN_0
#define BUTTON_GPIO_Port GPIOA
#define BUTTON_EXTI_IRQn EXTI0_1_IRQn
#define PWR_GOOD_Pin GPIO_PIN_1
#define PWR_GOOD_GPIO_Port GPIOA
#define PWR_GOOD_EXTI_IRQn EXTI0_1_IRQn
#define CAP_CHARGED_Pin GPIO_PIN_7
#define CAP_CHARGED_GPIO_Port GPIOA
#define NCHARGER_NBOOSTER_ENB_Pin GPIO_PIN_4
#define NCHARGER_NBOOSTER_ENB_GPIO_Port GPIOA
#define STATUS_LED_Pin GPIO_PIN_9
#define STATUS_LED_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define STATE_BACKUP_OFFSET		0

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
