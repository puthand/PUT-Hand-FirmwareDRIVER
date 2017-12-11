#ifndef SYSTEM_VARS_H_
#define SYSTEM_VARS_H_

#define MotorDriver_RSAddr_EEPROMAddr		0
uint8_t MotorDriver_RSAddr;

typedef struct
{
	uint16_t	POS_ADC_MaxValue;
	uint16_t	POS_ADC_MinValue;
}MotorDriver_Settings_TypeDef;

#define MotorDriver_Settings_EEPROMAddr		1
MotorDriver_Settings_TypeDef MotorDriver_Settings;

uint16_t ADC_data[2];
uint16_t ADC_Current;
uint16_t ADC_Pos;

#define USART_WatchDog_Limit	50
uint8_t USART_WatchDog_Counter;

typedef enum
{
	GET_STATE_ = 0x01,
	SET_PWM_ = 0x02,
	CALIBRATE_ = 0x03
}USART_CMD;

uint8_t USART_TX_buffer[33];


#endif /* SYSTEM_VARS_H_ */
