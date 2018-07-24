#include "main.h"

int main(void)
{
	Init();

	ADC_Pos_Raw_History_Ptr = 0;
	ADC_FirstCycle = 1;

	CurrentOperation_State = Operation_OK;

	EEPROM_Read_MotorDriver_RSAddr();
	EEPROM_Read_ADC_Pos_Count();
	EEPROM_Read_MotorDriver_Settings();

	SysTick_Init();
	CRC_Init();
	ADC_Init();
	DMA_Init();
	GPIO_Init();
	TIM_Init();
	USART_Init();

	while(1)
	{

	}
}
