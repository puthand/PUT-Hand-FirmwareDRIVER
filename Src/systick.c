#include "systick.h"

#include "base64.h"

void SysTick_Init(void)
{
	LL_Init1msTick(32000000);
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

	NVIC_SetPriority(SysTick_IRQn, 0);
	LL_SYSTICK_EnableIT();
}

void SysTick_Handler(void)
{
	USART_WatchDog_Counter++;
	if(USART_WatchDog_Counter >= USART_WatchDog_Limit)
	{
		USART_WatchDog_Counter = 0;
		TIM2->CCR1 = 0; //Stop motor if not receiving communication
		TIM2->CCR2 = 0;
	}
}
