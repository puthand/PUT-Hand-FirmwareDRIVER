#include "usart.h"

#include "base64.h"

static void USART_RS(void);

static LL_USART_InitTypeDef USART_InitStruct;

void USART_Init(void)
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

	USART_RS();
}

static uint8_t USART_RX_buffer[33];
static uint8_t USART_RX_Ptr;

void USART_RS(void)
{
	USART_RX_Ptr = 0;

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

	NVIC_SetPriority(USART2_IRQn, 1);
	NVIC_EnableIRQ(USART2_IRQn);

	LL_USART_EnableDMAReq_TX(USART2);

	LL_USART_EnableIT_RXNE(USART2);

	LL_USART_Enable(USART2);
}

static uint8_t USART_RX_Decoded[24];
static uint8_t USART_RX_Decoded_Len;

static uint8_t USART_TX_ToEncode[24];
static uint8_t USART_TX_ToEncode_Len;
static uint8_t USART_TX_Encoded_Len;

static uint16_t USART_TX_PWMValue;

//static uint8_t USART_Dst_Adr;
//static uint8_t USART_Cmd;
//static uint8_t USART_Payload_Size;


void USART2_IRQHandler(void)
{
	if(LL_USART_IsActiveFlag_RXNE(USART2))
	{
		while(LL_USART_IsActiveFlag_RXNE(USART2))
		{
			USART_RX_buffer[USART_RX_Ptr] = LL_USART_ReceiveData8(USART2);

			if(USART_RX_buffer[USART_RX_Ptr] == '\n')
			{
				if(USART_RX_Ptr > 1)//if any b64 data could be decoded
				{
					USART_RX_Decoded_Len = b64_decode(USART_RX_buffer, USART_RX_Ptr, USART_RX_Decoded);

					if(USART_RX_Decoded_Len >= 3)//id packet caintains at leas addr, cmd and crc
					{
						if(USART_RX_Decoded[0] == MotorDriver_RSAddr || USART_RX_Decoded[0] == 255) //if packet is addressed to this module or broadcast
						{
							USART_WatchDog_Counter = 0;

							LL_CRC_ResetCRCCalculationUnit(CRC);
							for(int i = 0; i < USART_RX_Decoded_Len - 1; i++)
							{
								LL_CRC_FeedData8(CRC, USART_RX_Decoded[i]);
							}

							if(USART_RX_Decoded[USART_RX_Decoded_Len-1] == LL_CRC_ReadData8(CRC)) //if CRC is ok
							{
								USART_TX_ToEncode[0] = 0x00; // Host
								USART_TX_ToEncode[1] = USART_RX_Decoded[1]; // CMD

								switch(USART_RX_Decoded[1])//CMD
								{
									case SET_PWM_: //new PWM + send status
										if(USART_RX_Decoded_Len >= 6)
										{
											((uint8_t*)&USART_TX_PWMValue)[0] = USART_RX_Decoded[2];
											((uint8_t*)&USART_TX_PWMValue)[1] = USART_RX_Decoded[3];

											if((USART_RX_Decoded[4] & 0x01) >> 0) //if freedrive enabled
											{
												TIM2->CCR1 = 1638;
												TIM2->CCR2 = 1638;
											}else
											{
												if((USART_RX_Decoded[4] & 0x02) >> 1) //if right or left
												{
													TIM2->CCR1 = USART_TX_PWMValue / 40;
													TIM2->CCR2 = 0;
												}else
												{
													TIM2->CCR1 = 0;
													TIM2->CCR2 = USART_TX_PWMValue / 40;
												}
											}
										}
									case GET_STATE_: //send status
										USART_TX_ToEncode[2] = ((uint8_t*)&ADC_Current)[0];
										USART_TX_ToEncode[3] = ((uint8_t*)&ADC_Current)[1];
										USART_TX_ToEncode[4] = ((uint8_t*)&ADC_Pos)[0];
										USART_TX_ToEncode[5] = ((uint8_t*)&ADC_Pos)[1];

										USART_TX_ToEncode_Len = 6;
										break;
									case CALIBRATE_: //calibrate
										//DO SHIT
										break;
								}

								LL_CRC_ResetCRCCalculationUnit(CRC);
								for(int i = 0; i< USART_TX_ToEncode_Len; i++)
								{
									LL_CRC_FeedData8(CRC, USART_TX_ToEncode[i]);
								}
								USART_TX_ToEncode[USART_TX_ToEncode_Len] = LL_CRC_ReadData8(CRC);
								USART_TX_ToEncode_Len++;

								USART_TX_Encoded_Len = b64_encode(USART_TX_ToEncode, USART_TX_ToEncode_Len, USART_TX_buffer);
								USART_TX_buffer[USART_TX_Encoded_Len] = '\n';
								USART_TX_Encoded_Len++;

								LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, USART_TX_Encoded_Len); //start USART_TX transfer
								LL_GPIO_SetOutputPin(RS_DRV_EN_GPIO_Port, RS_DRV_EN_Pin);
								LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
							}
						}
					}
				}

				USART_RX_Ptr = 0;
			}else
			{
				USART_RX_Ptr++;
				if(USART_RX_Ptr >= 33)
				{
					USART_RX_Ptr = 0;
				}
			}
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
