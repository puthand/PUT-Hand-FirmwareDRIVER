#ifndef SYSTEM_VARS_H_
#define SYSTEM_VARS_H_

#define MotorDriver_RSAddr_EEPROMAddr		0
volatile uint8_t MotorDriver_RSAddr;

typedef struct
{
	uint16_t	POS_ADC_MaxValue;
	uint16_t	POS_ADC_MinValue;
	uint8_t		POS_Invert;
}MotorDriver_Settings_TypeDef;

typedef enum
{
	Operation_OK = 0x00,
	Operation_Fault = 0x04
}Operation_State;

Operation_State CurrentOperation_State;

#define MotorDriver_Settings_EEPROMAddr		1
volatile MotorDriver_Settings_TypeDef MotorDriver_Settings;

#define ADC_CurrentScaler					38968 / 100000

volatile uint16_t ADC_data[2];
volatile uint16_t ADC_Current;

#define ADC_Pos_Raw_History_Size	5
volatile uint16_t ADC_Pos_Raw_History[ADC_Pos_Raw_History_Size];
volatile uint8_t ADC_Pos_Raw_History_Ptr;
volatile uint8_t ADC_FirstCycle;
volatile uint16_t ADC_Pos_Raw;
volatile uint16_t ADC_Pos;



#define USART_WatchDog_Limit				50
volatile uint8_t USART_WatchDog_Counter;

typedef enum
{
	SET_PWM_ = 0x01,
	GET_STATE_ = 0x02,
	CALIBRATE_ = 0x03
}USART_CMD;

#define USART_MaxBufferSize					32
#define USART_b64BufferSize					USART_MaxBufferSize * 6 / 8

volatile uint8_t USART_TX_buffer[USART_MaxBufferSize];

#define CalibrationDelay					0x23FFFFD

#define PWM_MaxValue						1638
#define PWM_CalibrationValue				700
#define PWM_Scaler							1 / 50

#endif /* SYSTEM_VARS_H_ */
