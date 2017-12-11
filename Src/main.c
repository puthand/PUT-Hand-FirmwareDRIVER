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

	//MotorDriver_Settings.POS_ADC_MaxValue = 4095;
	//MotorDriver_Settings.POS_ADC_MinValue = 0;
	//EEPROM_Write_MotorDriver_Settings();



	while (1)
	{
		volatile int i = 0;
		i = 1;
	}
}
