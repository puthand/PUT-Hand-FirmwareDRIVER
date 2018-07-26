#include "adc.h"

static void ADC_CUR_POS();

static LL_ADC_InitTypeDef ADC_InitStruct;
static LL_ADC_REG_InitTypeDef ADC_REG_InitStruct;

void ADC_Init(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

	ADC_CUR_POS();
}

void ADC_CUR_POS()
{
	ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);

	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_EXT_TIM21_CH2;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
	LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISINGFALLING);

	if(ADC_Pos_Count == 2)
		LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_4 | LL_ADC_CHANNEL_7 | LL_ADC_CHANNEL_5);
	else if(ADC_Pos_Count == 3)
		LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_4 | LL_ADC_CHANNEL_7 | LL_ADC_CHANNEL_5 | LL_ADC_CHANNEL_3);
	else if(ADC_Pos_Count == 4)
		LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_4 | LL_ADC_CHANNEL_7 | LL_ADC_CHANNEL_5 | LL_ADC_CHANNEL_3 | LL_ADC_CHANNEL_2);
	else
		LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_4 | LL_ADC_CHANNEL_7);

	LL_ADC_Enable(ADC1);

	LL_ADC_REG_StartConversion(ADC1);
}
