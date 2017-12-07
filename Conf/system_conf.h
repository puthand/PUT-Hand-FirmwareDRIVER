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

#define RS_DRV_EN_Pin LL_GPIO_PIN_14
#define RS_DRV_EN_GPIO_Port GPIOC
#define FAULT_Pin LL_GPIO_PIN_15
#define FAULT_GPIO_Port GPIOC
#define FAULT_EXTI_IRQn EXTI4_15_IRQn
#define PWM_1_Pin LL_GPIO_PIN_0
#define PWM_1_GPIO_Port GPIOA
#define PWM_2_Pin LL_GPIO_PIN_1
#define PWM_2_GPIO_Port GPIOA
#define ISEN_Pin LL_GPIO_PIN_4
#define ISEN_GPIO_Port GPIOA
#define POS_Pin LL_GPIO_PIN_7
#define POS_GPIO_Port GPIOA
#define UART_TX_Pin LL_GPIO_PIN_9
#define UART_TX_GPIO_Port GPIOA
#define UART_RX_Pin LL_GPIO_PIN_10
#define UART_RX_GPIO_Port GPIOA
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

#endif /* SYSTEM_CONF_H_ */
