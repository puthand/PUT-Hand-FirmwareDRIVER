#include "systick.h"

#include "base64.h"

void SysTick_Init(void)
{
	LL_Init1msTick(32000000);
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

	NVIC_SetPriority(SysTick_IRQn, 0);
	LL_SYSTICK_EnableIT();
}

static int i = 0;

void SysTick_Handler(void)
{
	USART_WatchDog_Counter++;
	if(USART_WatchDog_Counter >= USART_WatchDog_Limit)
	{
		USART_WatchDog_Counter = 0;
		TIM2->CCR1 = 0; //Stop motor if not receiving communication
		TIM2->CCR2 = 0;
	}

	/*static uint8_t USART_TX_ToEncode[24];
	static uint8_t USART_TX_Encoded_Len;

	//i = 10000;
	USART_TX_ToEncode[0] = 0x01;
	USART_TX_ToEncode[1] = SET_PWM_; // CMD
	USART_TX_ToEncode[2] = ((uint8_t*)&i)[0];
	USART_TX_ToEncode[3] = ((uint8_t*)&i)[1];
	USART_TX_ToEncode[4] = 0x02; //turn into higher value

	LL_CRC_ResetCRCCalculationUnit(CRC);
	LL_CRC_FeedData8(CRC, USART_TX_ToEncode[0]);
	LL_CRC_FeedData8(CRC, USART_TX_ToEncode[1]);
	LL_CRC_FeedData8(CRC, USART_TX_ToEncode[2]);
	LL_CRC_FeedData8(CRC, USART_TX_ToEncode[3]);
	LL_CRC_FeedData8(CRC, USART_TX_ToEncode[4]);
	USART_TX_ToEncode[5] = LL_CRC_ReadData8(CRC);

	USART_TX_Encoded_Len = b64_encode(USART_TX_ToEncode, 6, USART_TX_buffer);
	USART_TX_buffer[USART_TX_Encoded_Len] = '\n';
	USART_TX_Encoded_Len++;

	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, USART_TX_Encoded_Len); //start USART_TX transfer
	LL_GPIO_SetOutputPin(RS_DRV_EN_GPIO_Port, RS_DRV_EN_Pin);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);

	i++;
	if(i==65535)
	{
		i = 0;
	}*/
}
