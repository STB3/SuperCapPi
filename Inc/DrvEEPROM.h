/*
 * DrvEEPROM.h
 *
 *  Created on: 04.06.2020
 *      Author: Dirk
 *      http://madtracer.com/stm/eeprom-in-stm32-using-hal/
 */

#ifndef DRVEEPROM_H_
#define DRVEEPROM_H_

#include "main.h"

#define FLASH_TYPEERASEDATA_BYTE         (0x00U)  // Erase 1 byte
#define FLASH_TYPEERASEDATA_HALFWORD     (0x01U)  // Erase 2 bytes
#define FLASH_TYPEERASEDATA_WORD         (0x02U)  // Erase 4 bytes

#define EEPROM_SIZE 	128
#define QUEUE_ENTRIES 	(EEPROM_SIZE / sizeof(QUEUE_ENTRY))
#define EEPROM_BASE_ADDR_WL				  0x08080004	// eePROM start with wear leveling

//#define EEPROM_BASE_ADDR				  0x08080000	// old eeprom start

//void writeToEEPROM (uint32_t address, uint32_t value);
//uint32_t readFromEEPROM (uint32_t address);
uint32_t load_eePROMqueue(void);
void write_eePROMvalue(uint32_t v);

struct
{
  uint32_t sequence_no;
  uint32_t power_state;
} QUEUE_ENTRY;


#endif /* DRVEEPROM_H_ */
