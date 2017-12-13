#include "main.h"

int main(void)
{
	Init();

	EEPROM_Read_MotorDriver_RSAddr();
	EEPROM_Read_MotorDriver_Settings();

	SysTick_Init();
	CRC_Init();
	ADC_Init();
	DMA_Init();
	GPIO_Init();
	TIM_Init();
	USART_Init();


	/*static uint8_t USART_TX_ToEncode[24];
	static uint8_t USART_TX_Encoded_Len;

	USART_TX_ToEncode[0] = 0x01;
	USART_TX_ToEncode[1] = CALIBRATE_; // CMD

	LL_CRC_ResetCRCCalculationUnit(CRC);
	LL_CRC_FeedData8(CRC, USART_TX_ToEncode[0]);
	LL_CRC_FeedData8(CRC, USART_TX_ToEncode[1]);
	USART_TX_ToEncode[2] = LL_CRC_ReadData8(CRC);

	USART_TX_Encoded_Len = b64_encode(USART_TX_ToEncode, 3, USART_TX_buffer);
	USART_TX_buffer[USART_TX_Encoded_Len] = '\n';
	USART_TX_Encoded_Len++;

	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, USART_TX_Encoded_Len); //start USART_TX transfer
	LL_GPIO_SetOutputPin(RS_DRV_EN_GPIO_Port, RS_DRV_EN_Pin);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);*/

	while(1)
	{

	}
}
