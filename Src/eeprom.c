/*
 * eeprom.c
 *
 *  Created on: 11.12.2017
 *      Author: Tomasz
 */

#include "eeprom.h"

static void EEPROM_Unlock();
static void EEPROM_Lock();

void EEPROM_Read_MotorDriver_RSAddr()
{
	MotorDriver_RSAddr =  *(uint8_t*)(DATA_EEPROM_BASE + MotorDriver_RSAddr_EEPROMAddr);
}

void EEPROM_Read_ADC_Pos_Count()
{
	ADC_Pos_Count =  *(uint8_t*)(DATA_EEPROM_BASE + ADC_Pos_Count_EEPROMAddr);
	if(ADC_Pos_Count > ADC_Pos_Count_Max)
	{
		ADC_Pos_Count = ADC_Pos_Count_Max;
	}
	if(ADC_Pos_Count == 0)
	{
		ADC_Pos_Count = 1;
	}
}

void EEPROM_Write_MotorDriver_Settings()
{
	EEPROM_Unlock();

	for(int i=0; i < sizeof(MotorDriver_Settings); i++)
	{
		*(uint8_t *)(DATA_EEPROM_BASE + MotorDriver_Settings_EEPROMAddr + i) = ((uint8_t*)&MotorDriver_Settings)[i];
	}

	EEPROM_Lock();
}

void EEPROM_Read_MotorDriver_Settings()
{
	for(int i=0; i < sizeof(MotorDriver_Settings); i++)
	{
		((uint8_t*)&MotorDriver_Settings)[i] = *(uint8_t *)(DATA_EEPROM_BASE + MotorDriver_Settings_EEPROMAddr + i);
	}
}

void EEPROM_Unlock(void)
{
   while ((FLASH->SR & FLASH_SR_BSY) != 0); /* Wait for FLASH to be free */

   if ((FLASH->PECR & FLASH_PECR_PELOCK) != 0) /* If PELOCK is locked */
   {
      FLASH->PEKEYR = 0x89ABCDEF; /* Unlock PELOCK */
      FLASH->PEKEYR = 0x02030405;
   }

   FLASH->PECR = FLASH->PECR | (FLASH_PECR_ERRIE | FLASH_PECR_EOPIE); /* enable flash interrupts */

   FLASH->PECR = FLASH->PECR & ~(FLASH_PECR_ERASE | FLASH_PECR_DATA); /* Reset the ERASE and DATA */
}

void EEPROM_Lock(void)
{
   while ((FLASH->SR & FLASH_SR_BSY) != 0); /* Wait for FLASH to be free */

   FLASH->PECR = FLASH->PECR & ~(FLASH_PECR_ERRIE | FLASH_PECR_EOPIE); /* disable flash interrupts */
   FLASH->PECR = FLASH->PECR | FLASH_PECR_PELOCK; /* Lock memory with PELOCK */
}


