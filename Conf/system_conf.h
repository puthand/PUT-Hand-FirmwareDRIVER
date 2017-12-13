/*
 * system_conf.h
 *
 *  Created on: 06.12.2017
 *      Author: Tomasz
 */

#ifndef SYSTEM_CONF_H_
#define SYSTEM_CONF_H_

#include "stm32l0xx.h"

#include "system_vars.h"

#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_crc.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_crs.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_gpio.h"

#define RS_DRV_EN_Pin 				LL_GPIO_PIN_14
#define RS_DRV_EN_GPIO_Port 		GPIOC

#define FAULT_Pin 					LL_GPIO_PIN_15
#define FAULT_GPIO_Port 			GPIOC
#define FAULT_EXTI_IRQn 			EXTI4_15_IRQn

#define PWM_1_Pin 					LL_GPIO_PIN_0
#define PWM_1_GPIO_Port 			GPIOA
#define PWM_2_Pin 					LL_GPIO_PIN_1
#define PWM_2_GPIO_Port 			GPIOA

#define ISEN_Pin 					LL_GPIO_PIN_4
#define ISEN_GPIO_Port 				GPIOA

#define POS_Pin 					LL_GPIO_PIN_7
#define POS_GPIO_Port 				GPIOA

#define UART_TX_Pin 				LL_GPIO_PIN_9
#define UART_TX_GPIO_Port 			GPIOA
#define UART_RX_Pin 				LL_GPIO_PIN_10
#define UART_RX_GPIO_Port 			GPIOA

#define ADC_CurrentScaler			38968 / 100000

#define CalibrationDelay			0xFFFFFF

#define PWM_MaxValue				1638
#define PWM_CalibrationValue		200
#define PWM_Scaler					1 / 40

#endif /* SYSTEM_CONF_H_ */
