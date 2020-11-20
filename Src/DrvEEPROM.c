/*
 * DrvEEPROM.c
 *
 *  Created on: 04.06.2020
 *      Author: Dirk
 */
#include "DrvEEPROM.h"


uint32_t last_sequence_no;
uint32_t queue_tail;
uint32_t current_value;


static void read_EEPROM(  uint32_t address, uint32_t *pData, uint32_t no_of_words);
static void write_EEPROM( uint32_t address, uint32_t *pData, uint32_t no_of_words);
static uint32_t readFromEEPROM (uint32_t address);
static void writeToEEPROM (uint32_t address, uint32_t value);


uint32_t readFromEEPROM (uint32_t address)
{
      return (*(__IO uint32_t *)address);
}


void writeToEEPROM (uint32_t address, uint32_t value)
{
  HAL_StatusTypeDef flash_ok = HAL_ERROR;
  while (flash_ok != HAL_OK)
  {
    flash_ok = HAL_FLASHEx_DATAEEPROM_Unlock();
  }
  flash_ok = HAL_ERROR;
  while (flash_ok != HAL_OK)
  {
    flash_ok = HAL_FLASHEx_DATAEEPROM_Erase (address); // Erase one word (4 bytes)
  }
  flash_ok = HAL_ERROR;
  while (flash_ok != HAL_OK)
  {
    flash_ok = HAL_FLASHEx_DATAEEPROM_Program (FLASH_TYPEPROGRAMDATA_WORD, address, value);
  }
  flash_ok = HAL_ERROR;
  while (flash_ok != HAL_OK)
  {
    flash_ok = HAL_FLASHEx_DATAEEPROM_Lock ();
  }
}


void read_EEPROM(  uint32_t address, uint32_t *pData, uint32_t no_of_words)
{
	uint32_t i;
	for(i = 0; i < no_of_words; i++)
	{
		*pData = (uint32_t *)readFromEEPROM(address + 4*i);
		pData++;
	}
}


void write_EEPROM( uint32_t address, uint32_t *pData, uint32_t no_of_words)
{
	uint32_t i;
	for(i = 0; i < no_of_words; i++)
	{
		writeToEEPROM(address + 4*i, *pData);
		pData++;
	}
}

// Called at startup
uint32_t load_eePROMqueue(void)
{
  int i;

  last_sequence_no = 0; // STM32l011 eeprom defaults to 0x00 instead of 0xFF
  queue_tail = 0;
  current_value = 0;
  for (i=0; i < QUEUE_ENTRIES; i++)
  {
    read_EEPROM(i * sizeof(QUEUE_ENTRY) + EEPROM_BASE_ADDR_WL, &QUEUE_ENTRY, sizeof(QUEUE_ENTRY)/4);
    if ((QUEUE_ENTRY.sequence_no > last_sequence_no) && (QUEUE_ENTRY.sequence_no != 0x00000000)) // Check!!
    {
      queue_tail = i;
      last_sequence_no = QUEUE_ENTRY.sequence_no;
      current_value = QUEUE_ENTRY.power_state;
    }
  }
  return current_value;
}


void write_eePROMvalue(uint32_t v)
{
  queue_tail++;
  if (queue_tail >= QUEUE_ENTRIES)
    queue_tail = 0;
  last_sequence_no++;
  QUEUE_ENTRY.sequence_no = last_sequence_no;
  QUEUE_ENTRY.power_state = v;
  write_EEPROM(queue_tail * sizeof(QUEUE_ENTRY) + EEPROM_BASE_ADDR_WL, &QUEUE_ENTRY, sizeof(QUEUE_ENTRY)/4);
  current_value = v;
}



/*
Example:

https://electronics.stackexchange.com/questions/60342/wear-leveling-on-a-microcontrollers-eeprom

Then when booting the largest sequence number can be used to determine both the next sequence number
to be used and the current tail of the queue. The following C pseudo-code demonstrates, this assumes
that upon initial programming the EEPROM area has been erased to values of 0xFF so I ignore a sequence number of 0xFFFF

struct
{
  uint32_t sequence_no;
  uint16_t my_data;
} QUEUE_ENTRY;

#define EEPROM_SIZE 128
#define QUEUE_ENTRIES (EEPROM_SIZE / sizeof(QUEUE_ENTRY))

uint32_t last_sequence_no;
uint8_t queue_tail;
uint16_t current_value;

// Called at startup
void load_queue()
{
  int i;

  last_sequence_no = 0;
  queue_tail = 0;
  current_value = 0;
  for (i=0; i < QUEUE_ENTRIES; i++)
  {
    // Following assumes you've written a function where the parameters
    // are address, pointer to data, bytes to read
    read_EEPROM(i * sizeof(QUEUE_ENTRY), &QUEUE_ENTRY, sizeof(QUEUE_ENTRY));
    if ((QUEUE_ENTRY.sequence_no > last_sequence_no) && (QUEUE_ENTRY.sequence_no != 0xFFFF))
    {
      queue_tail = i;
      last_sequence_no = QUEUE_ENTRY.sequence_no;
      current_value = QUEUE_ENTRY.my_data;
    }
  }
}

void write_value(uint16_t v)
{
  queue_tail++;
  if (queue_tail >= QUEUE_ENTRIES)
    queue_tail = 0;
  last_sequence_no++;
  QUEUE_ENTRY.sequence_no = last_sequence_no;
  QUEUE_ENTRY.my_data = v;
  // Following assumes you've written a function where the parameters
  // are address, pointer to data, bytes to write
  write_EEPROM(queue_tail * sizeof(QUEUE_ENTRY), &QUEUE_ENTRY, sizeof(QUEUE_ENTRY));
  current_value = v;
}
*/
