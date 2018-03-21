#ifndef __UTIL_EEPROM__
#define __UTIL_EEPROM__

#include "UTIL_STD.h"
#include "UTIL_constant.h"
#include "UTIL_constant_program.h"

uint8_t EEPROM_ReadByte(uint32_t addr);
uint32_t EEPROM_ReadInt(uint32_t addr);
void EEPROM_ReadNBytes(uint32_t addr, uint8_t *buffer, uint8_t len);
void EEPROM_WriteByte(uint32_t addr, uint8_t data);
void EEPROM_WriteNBytes(uint32_t addr, uint8_t *buffer, uint8_t len);
uint32_t flash_ReadFirmwareInt(uint32_t addr);
void flash_WriteFirmwareInt(uint32_t addr, uint32_t data);
void EEPROM_FactoryReset(void);
void EEPROM_HostReset(void);
void EEPROM_EraseNBytes(uint32_t addr, uint8_t len);

void EEPROM_ReadLinkStatus(uint8_t *linkObject);

// FLASH
#define FLASH_USER_START_ADDR 0x8001000
#define NEW_FIRMWARE_ADDRESS 0x8008800 // 64K

// EEPROM
#define FACTORY_RESET_SIZE 0x110 //(30+1)*8+1+5*4=269

#define EEPROM_BASE 0x8080000
#define HUB_ADDRESS 0x8080000
#define IDADDRESS (0x8000ff0)

#define DEVICE_LIST_START_ADDRESS (HUB_ADDRESS + 8) // 0x8080008
#define DEVICE_LIST_SIZE 0xF0

// EEPROM - SCHEDULE
#define SCHEDULE_STATUS (EEPROM_BASE + DEVICE_LIST_SIZE)
#define SCHEDULE_BASE (SCHEDULE_STATUS + 1)

#define SUN_RISE_SET_ADDR (SCHEDULE_BASE + 6 * 6) // 6 is max schedules
#define POWERON_STATUS (SUN_RISE_SET_ADDR + 4)
#define SWITCH_VALUE (POWERON_STATUS + 1)

// EEPROM -FIRMWARE UPDATE
#define FIRMWARE_SIZE_ADDRESS (EEPROM_BASE + 4000)
#define FIRMWARE_CHECKSUM_ADDRESS (FIRMWARE_SIZE_ADDRESS + 4)
#define FIRMWARE_EXPECTED_SEQUENCE_ADDRESS (FIRMWARE_CHECKSUM_ADDRESS + 4)
// 00: No update; 01: In updating; 03: bootloader should start it up
#define FIRMWARE_STATE_ADDRESS (FIRMWARE_EXPECTED_SEQUENCE_ADDRESS + 4)
#define FIRMWARE_LAST_SAVED_SEQUENCE_ADDRESS (FIRMWARE_STATE_ADDRESS + 4)

#endif