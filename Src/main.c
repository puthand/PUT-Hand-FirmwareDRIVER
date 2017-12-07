#include "main.h"

int main(void)
{
	Init();

	SysTick_Init();
	DMA_Init();
	GPIO_Init();
	TIM_Init();
	ADC_Init();
	USART_Init();
	CRC_Init();

	RX_Ptr = 0;
	for(int i=0; i<10; i++)
	{
		USART_TX_buffer[i] = i;
	}

	/*LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, 10); //start USART_TX transfer
	LL_GPIO_SetOutputPin(RS_DRV_EN_GPIO_Port, RS_DRV_EN_Pin);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);*/

	while (1)
	{
		volatile int i = 0;
		i = 1;
	}
}
