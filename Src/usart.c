#include "usart.h"

static void USART_RS(void);

static LL_USART_InitTypeDef USART_InitStruct;

void USART_Init(void)
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

	USART_RS();
}

void USART_RS(void)
{
	USART_InitStruct.BaudRate = 115200;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART2, &USART_InitStruct);

	LL_USART_DisableOverrunDetect(USART2);
	LL_USART_ConfigAsyncMode(USART2);

	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);

	LL_USART_EnableDMAReq_TX(USART2);

	LL_USART_EnableIT_RXNE(USART2);

	LL_USART_Enable(USART2);
}

void USART2_IRQHandler(void)
{
	if(LL_USART_IsActiveFlag_RXNE(USART2))
	{
		while(LL_USART_IsActiveFlag_RXNE(USART2))
		{
			USART_RX_buffer[USART_RX_Ptr] = LL_USART_ReceiveData8(USART2);
			USART_RX_Ptr++;
		}
	}

	if(LL_USART_IsActiveFlag_TXE(USART2) && LL_USART_IsEnabledIT_TXE(USART2))
	{
		LL_USART_DisableIT_TXE(USART2);
		LL_USART_EnableIT_TC(USART2);
	}

	if(LL_USART_IsActiveFlag_TC(USART2) && LL_USART_IsEnabledIT_TC(USART2))
	{
		LL_GPIO_ResetOutputPin(RS_DRV_EN_GPIO_Port, RS_DRV_EN_Pin);//disable RS trasmitter

		LL_USART_ClearFlag_TC(USART2);
		LL_USART_DisableIT_TC(USART2);
	}
}
