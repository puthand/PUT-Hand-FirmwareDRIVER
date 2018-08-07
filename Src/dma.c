#include "dma.h"

static void DMA_ADC();
static void DMA_USART();

static LL_DMA_InitTypeDef DMA_InitStruct;

void DMA_Init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	DMA_ADC();
	DMA_USART();
}

void DMA_ADC()
{
	DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&ADC_raw;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_CIRCULAR;
	DMA_InitStruct.NbData = ADC_Pos_Count+1;
	DMA_InitStruct.PeriphOrM2MSrcAddress = LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA);
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = LL_DMA_REQUEST_0;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_MEDIUM;
	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &DMA_InitStruct);

	NVIC_SetPriority(DMA1_Channel1_IRQn, 2);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

static void DMA_USART()
{
	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&USART_TX_buffer;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
	DMA_InitStruct.NbData = 0;
	DMA_InitStruct.PeriphOrM2MSrcAddress = LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT);
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = LL_DMA_REQUEST_4;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_2, &DMA_InitStruct);

	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

	LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_2);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
}

static volatile uint32_t ADC_Pos_Raw_Tmp;
static volatile uint16_t ADC_Pos_Calulation_Tmp;

static volatile int32_t ADC_Work_Calc_Tmp;

void DMA1_Channel1_IRQHandler(void)
{
	if(LL_DMA_IsActiveFlag_TC1(DMA1))
	{
		//                                                 3300 [mV]
		//                                          ADC * ---------
		//           Vout                                   4095
		//Is = ----------------- [mA] = ----------------------------------
		//           Rgain                              47000 [Ohm]
		//     Rs * -------             0,220 [mOhm] * --------------
		//            Rin                                5000 [Ohm]

		if(ADC_Pos_Count == 2)
		{
			ADC_data[0] = ADC_raw[0]; //ISEN
			ADC_data[1] = ADC_raw[2]; //POS1
			ADC_data[2] = ADC_raw[1]; //POS2
		}else if(ADC_Pos_Count == 3)
		{
			ADC_data[0] = ADC_raw[1]; //ISEN
			ADC_data[1] = ADC_raw[3]; //POS1
			ADC_data[2] = ADC_raw[2]; //POS2
			ADC_data[3] = ADC_raw[0]; //POS3
		}else if(ADC_Pos_Count == 4)
		{
			ADC_data[0] = ADC_raw[2]; //ISEN
			ADC_data[1] = ADC_raw[4]; //POS1
			ADC_data[2] = ADC_raw[3]; //POS2
			ADC_data[3] = ADC_raw[1]; //POS3
			ADC_data[4] = ADC_raw[0]; //POS4
		}else
		{
			ADC_data[0] = ADC_raw[0]; //ISEN
		    ADC_data[1] = ADC_raw[1]; //POS1
		}

		ADC_Current = (uint16_t)(ADC_data[0] * ADC_CurrentScaler);

		if(ADC_FirstCycle == 1)
		{
			for(int i=0; i<ADC_Pos_Raw_History_Size; i++)
			{
				for(int p=0; p<ADC_Pos_Count; p++)
				{
					ADC_Pos_Raw_History[p][i] = ADC_data[p+1];
				}
			}

			for(int i=0; i<ADC_Work_Velocity_History_Size; i++)
			{
				ADC_Work_Velocity_History[i] = 0;
			}
			ADC_Work_Flag = 0;
			if(ADC_Pos_Count == 1)
			{
				ADC_Work_Pos_Raw_Prev = ADC_data[1];
			}else
			{
				ADC_Work_Pos_Raw_Prev = ADC_data[2];
			}


			ADC_FirstCycle = 0;
		}else
		{
			for(int p=0; p<ADC_Pos_Count; p++)
			{
				ADC_Pos_Raw_History[p][ADC_Pos_Raw_History_Ptr] = ADC_data[p+1];
			}
			ADC_Pos_Raw_History_Ptr++;
			if(ADC_Pos_Raw_History_Ptr == ADC_Pos_Raw_History_Size)
			{
				ADC_Pos_Raw_History_Ptr = 0;
			}
		}

		for(int p=0; p<ADC_Pos_Count; p++)
		{
			ADC_Pos_Raw_Tmp = 0;
			for(int i=0; i<ADC_Pos_Raw_History_Size; i++)
			{
				ADC_Pos_Raw_Tmp += ADC_Pos_Raw_History[p][i];
			}
			ADC_Pos_Raw[p] = ADC_Pos_Raw_Tmp / ADC_Pos_Raw_History_Size;

			ADC_Pos_Calulation_Tmp = ADC_Pos_Raw[p];

			if(ADC_Pos_Calulation_Tmp < MotorDriver_Settings.POS_ADC_MinValue[p])
			{
				ADC_Pos_Calulation_Tmp = MotorDriver_Settings.POS_ADC_MinValue[p];
			}

			if(ADC_Pos_Calulation_Tmp > MotorDriver_Settings.POS_ADC_MaxValue[p])
			{
				ADC_Pos_Calulation_Tmp = MotorDriver_Settings.POS_ADC_MaxValue[p];
			}

			ADC_Pos[p] = (ADC_Pos_Calulation_Tmp - MotorDriver_Settings.POS_ADC_MinValue[p]) * 65535 / (MotorDriver_Settings.POS_ADC_MaxValue[p] - MotorDriver_Settings.POS_ADC_MinValue[p]);
		}

		if(ADC_Pos_Count == 1)
		{
			ADC_Work_Velocity_History[ADC_Work_Velocity_History_Ptr] = ADC_Pos_Raw[0] - ADC_Work_Pos_Raw_Prev;
			ADC_Work_Pos_Raw_Prev = ADC_Pos_Raw[0];
		}else
		{
			ADC_Work_Velocity_History[ADC_Work_Velocity_History_Ptr] = ADC_Pos_Raw[1] - ADC_Work_Pos_Raw_Prev;
			ADC_Work_Pos_Raw_Prev = ADC_Pos_Raw[1];
		}
		ADC_Work_Velocity_History_Ptr++;
		if(ADC_Work_Velocity_History_Ptr == ADC_Work_Velocity_History_Size)
		{
			ADC_Work_Velocity_History_Ptr = 0;
		}

		ADC_Work_Calc_Tmp = 0;
		for(int i=0; i<ADC_Work_Velocity_History_Size; i++)
		{
			ADC_Work_Calc_Tmp += ADC_Work_Velocity_History[i];
		}
		ADC_Work_Calc_Tmp /= ADC_Work_Velocity_History_Size;

		if(ADC_Work_Calc_Tmp > ADC_Work_Velocity_DeadZone)
		{
			ADC_Work_Flag = 1;
		}else if(ADC_Work_Calc_Tmp < -ADC_Work_Velocity_DeadZone)
		{
			ADC_Work_Flag = -1;
		}else
		{
			ADC_Work_Flag = 0;
		}

		LL_DMA_ClearFlag_TC1(DMA1);
	}
}

void DMA1_Channel2_3_IRQHandler(void)
{
	if(LL_DMA_IsActiveFlag_HT2(DMA1))
	{
		LL_USART_EnableIT_TXE(USART2); //enable usart irq to monitor transfer completion

		LL_DMA_ClearFlag_HT2(DMA1);
	}

	if(LL_DMA_IsActiveFlag_TC2(DMA1))
	{
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);

		LL_DMA_ClearFlag_TC2(DMA1);
	}
}
