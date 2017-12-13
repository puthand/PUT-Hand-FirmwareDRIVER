#ifndef SYSTEM_VARS_H_
#define SYSTEM_VARS_H_

#define MotorDriver_RSAddr_EEPROMAddr		0
uint8_t MotorDriver_RSAddr;

typedef struct
{
	uint16_t	POS_ADC_MaxValue;
	uint16_t	POS_ADC_MinValue;
	uint8_t		POS_Invert;
}MotorDriver_Settings_TypeDef;

#define MotorDriver_Settings_EEPROMAddr		1
MotorDriver_Settings_TypeDef MotorDriver_Settings;

#define ADC_CurrentScaler					38968 / 100000

volatile uint16_t ADC_data[2];
volatile uint16_t ADC_Current;
volatile uint16_t ADC_Pos_Raw;
volatile uint16_t ADC_Pos;

#define USART_WatchDog_Limit				50
uint8_t USART_WatchDog_Counter;

typedef enum
{
	SET_PWM_ = 0x01,
	GET_STATE_ = 0x02,
	CALIBRATE_ = 0x03
}USART_CMD;

#define USART_MaxBufferSize					32
#define USART_b64BufferSize					USART_MaxBufferSize * 6 / 8

uint8_t USART_TX_buffer[USART_MaxBufferSize];

#define CalibrationDelay					0xFFFFFF

#define PWM_MaxValue						1638
#define PWM_CalibrationValue				200
#define PWM_Scaler							1 / 40


#endif /* SYSTEM_VARS_H_ */
