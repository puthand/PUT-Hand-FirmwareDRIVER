/*
 * eeprom.h
 *
 *  Created on: 11.12.2017
 *      Author: Tomasz
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "system_conf.h"

void EEPROM_Read_MotorDriver_RSAddr();

void EEPROM_Read_ADC_Pos_Count();

void EEPROM_Write_MotorDriver_Settings();
void EEPROM_Read_MotorDriver_Settings();

#endif /* EEPROM_H_ */
