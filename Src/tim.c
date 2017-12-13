#include "tim.h"

static void TIM_PWM(void);
static void TIM_ADC_TRIG(void);

static LL_TIM_InitTypeDef TIM_InitStruct;
static LL_TIM_OC_InitTypeDef TIM_OC_InitStruct;

void TIM_Init(void)
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM21);

	TIM_PWM();
	TIM_ADC_TRIG();
}

void TIM_PWM(void)
{
	TIM_InitStruct.Prescaler = 0;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = PWM_MaxValue; // 65535 / 40
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIM2, &TIM_InitStruct);

	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = 0;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;
	LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);

	TIM_OC_InitStruct.CompareValue = 0;
	LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);

	LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH2);

	LL_TIM_EnableCounter(TIM2);
}

void TIM_ADC_TRIG(void)
{
	TIM_InitStruct.Prescaler = 31;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 999;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIM21, &TIM_InitStruct);

	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_TOGGLE; //needed to set input in order to ADC trigger from CC2 to work
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = 999;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	LL_TIM_OC_Init(TIM21, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);

	LL_TIM_EnableCounter(TIM21);
	LL_TIM_CC_EnableChannel(TIM21, LL_TIM_CHANNEL_CH2);
}
