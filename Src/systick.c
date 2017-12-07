#include "systick.h"

void SysTick_Init(void)
{
	LL_Init1msTick(32000000);
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

	NVIC_SetPriority(SysTick_IRQn, 0);
	LL_SYSTICK_EnableIT();
}

void SysTick_Handler(void)
{
	//LL_GPIO_SetOutputPin(RS_DRV_EN_GPIO_Port, RS_DRV_EN_Pin);
	//LL_USART_TransmitData8(USART2, 0xBB);
	//while(!LL_USART_IsActiveFlag_TC(USART2));
	//LL_USART_ClearFlag_TC(USART2);
}
