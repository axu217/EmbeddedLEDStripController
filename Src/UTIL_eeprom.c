#include "UTIL_eeprom.h"

uint8_t EEPROM_ReadByte(uint32_t addr) { return *(uint8_t *) (addr); }

uint32_t EEPROM_ReadInt(uint32_t addr) { return *(uint32_t *) (addr); }

void EEPROM_ReadNBytes(uint32_t addr, uint8_t *buffer, uint8_t len) {
    while (len--) {
        *buffer++ = EEPROM_ReadByte(addr++);
    }
}

void EEPROM_WriteByte(uint32_t addr, uint8_t data) {
    HAL_FLASHEx_DATAEEPROM_Unlock();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, addr, data);
    HAL_FLASHEx_DATAEEPROM_Lock();
}

void EEPROM_WriteNBytes(uint32_t addr, uint8_t *buffer, uint8_t len) {
    HAL_FLASHEx_DATAEEPROM_Unlock();
    while (len--) {
        HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, addr++, *buffer++);
    }
    HAL_FLASHEx_DATAEEPROM_Lock();
}

void EEPROM_EraseNBytes(uint32_t addr, uint8_t len) {
    HAL_FLASHEx_DATAEEPROM_Unlock();
    while (len--) {
        HAL_FLASHEx_DATAEEPROM_Erase(FLASH_TYPEERASEDATA_BYTE, addr++);
    }
    HAL_FLASHEx_DATAEEPROM_Lock();
}

void EEPROM_FactoryReset(void) {
    uint16_t i;
    HAL_FLASHEx_DATAEEPROM_Unlock();
    for (i = 0; i < (FACTORY_RESET_SIZE / 4); i++) {
        HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, HUB_ADDRESS + i * 4, 0x00000000);
    }
    HAL_FLASHEx_DATAEEPROM_Lock();
}

void EEPROM_HostReset(void) {
    uint16_t i;
    HAL_FLASHEx_DATAEEPROM_Unlock();
    for (i = 0; i < 2; i++) {
        HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, HUB_ADDRESS + i * 4, 0x00000000);
    }
    HAL_FLASHEx_DATAEEPROM_Lock();
}

void EEPROM_ReadLinkStatus(uint8_t *linkObject) {
    EEPROM_ReadNBytes(DEVICE_LIST_START_ADDRESS, linkObject, 8);
}
