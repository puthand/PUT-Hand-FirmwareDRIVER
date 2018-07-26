#include "gpio.h"

static void GPIO_DRV_EN();
static void GPIO_MOT_FAULT();
static void GPIO_PWM();
static void GPIO_USART();
static void GPIO_ADC();

static LL_EXTI_InitTypeDef EXTI_InitStruct;
static LL_GPIO_InitTypeDef GPIO_InitStruct;

void GPIO_Init(void)
{
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	GPIO_DRV_EN();
	GPIO_MOT_FAULT();
	GPIO_PWM();
	GPIO_USART();
	GPIO_ADC();
}

void GPIO_DRV_EN()
{
	GPIO_InitStruct.Pin = RS_DRV_EN_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(RS_DRV_EN_GPIO_Port, &GPIO_InitStruct);

	LL_GPIO_ResetOutputPin(RS_DRV_EN_GPIO_Port, RS_DRV_EN_Pin);
}

void GPIO_MOT_FAULT()
{
	GPIO_InitStruct.Pin = FAULT_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(FAULT_GPIO_Port, &GPIO_InitStruct);

	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE15);

	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_15;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
	LL_EXTI_Init(&EXTI_InitStruct);

	NVIC_SetPriority(EXTI4_15_IRQn, 1);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_15);
}

void GPIO_PWM()
{
	GPIO_InitStruct.Pin = PWM_1_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
	LL_GPIO_Init(PWM_1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = PWM_2_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
	LL_GPIO_Init(PWM_2_GPIO_Port, &GPIO_InitStruct);
}

void GPIO_USART()
{
	GPIO_InitStruct.Pin = UART_TX_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
	LL_GPIO_Init(UART_TX_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = UART_RX_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
	LL_GPIO_Init(UART_RX_GPIO_Port, &GPIO_InitStruct);
}

static void GPIO_ADC()
{
	GPIO_InitStruct.Pin = ISEN_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(ISEN_GPIO_Port, &GPIO_InitStruct);

	if(ADC_Pos_Count == 2)
		GPIO_InitStruct.Pin = LL_GPIO_PIN_7 | LL_GPIO_PIN_5;
	else if(ADC_Pos_Count == 3)
		GPIO_InitStruct.Pin = LL_GPIO_PIN_7 | LL_GPIO_PIN_5 | LL_GPIO_PIN_3;
	else if(ADC_Pos_Count == 4)
		GPIO_InitStruct.Pin = LL_GPIO_PIN_7 | LL_GPIO_PIN_5 | LL_GPIO_PIN_3 | LL_GPIO_PIN_2;
	else
		GPIO_InitStruct.Pin = LL_GPIO_PIN_7;

	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(POS_GPIO_Port, &GPIO_InitStruct);
}

void EXTI4_15_IRQHandler(void)
{
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_15) != RESET)
	{
		TIM2->CCR1 = 0;//stop motor on fault
		TIM2->CCR2 = 0;
		CurrentOperation_State = Operation_Fault;

		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_15);
	}
}
