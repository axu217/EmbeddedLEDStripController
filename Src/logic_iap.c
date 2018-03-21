#include "logic_iap.h"

uint16_t totalSequences = 0;
uint16_t sequenceToSend = 0;
bool IAP_enabled = false;
bool processedExpected = true;

uint16_t IAPtimeoutCounter;
static const uint16_t IAP_TIMEOUT_LIMIT = 3000;

uint16_t IAPRetryCounter;
static const uint16_t IAP_RETRY_LIMIT = 3;

void IAP_init() {
    uint8_t status = EEPROM_ReadByte(FIRMWARE_STATE_ADDRESS);

    if (status == 0x01) {

        uint8_t sizeBuffer;
        uint8_t checksumBuffer;
        EEPROM_ReadNBytes(FIRMWARE_SIZE_ADDRESS, &sizeBuffer, 4);
        EEPROM_ReadNBytes(FIRMWARE_CHECKSUM_ADDRESS, &checksumBuffer, 4);

        IAP_startIAP(&sizeBuffer, &checksumBuffer);
    }
}
void IAP_Loop() {

    // No need for IAP
    if (IAP_enabled == false) {
        return;
    }
    // You're done!
    if (sequenceToSend - 1 == totalSequences) {
        IAP_enabled = false;
        EEPROM_WriteByte(FIRMWARE_STATE_ADDRESS, 0x03);
        HAL_NVIC_SystemReset();
        return;
    }

    IAPtimeoutCounter++;

    // processExpected is the lock
    if (processedExpected == true) {
        uint8_t sequenceHighByte = sequenceToSend / 16;
        uint8_t sequenceLowByte = sequenceToSend % 16;

        uint8_t toSend[24];
        FORMATTER_getFirmware(toSend, sequenceHighByte, sequenceLowByte);
        queue_addFromBuffer(toSend, true);
        // lock from sending more
        // start timer
        IAPtimeoutCounter = 0;
        processedExpected = false;
        return;
    }

    if (IAPtimeoutCounter > IAP_TIMEOUT_LIMIT) {
        IAPtimeoutCounter = 0;
        processedExpected = true;
        IAPRetryCounter++;
    }

    if (IAPRetryCounter > IAP_RETRY_LIMIT) {
        IAP_enabled = false;
        EEPROM_WriteByte(FIRMWARE_STATE_ADDRESS, 0x00);
    }

    return;
}

void IAP_process(uint8_t *response) {

    uint8_t responseLow = response[11];
    uint8_t expectedLow = sequenceToSend % 16;

    if (responseLow != expectedLow) {
        uint8_t sequenceHighByte = sequenceToSend / 16;
        uint8_t sequenceLowByte = sequenceToSend % 16;

        uint8_t toSend[24];
        FORMATTER_getFirmware(toSend, sequenceHighByte, sequenceLowByte);
        queue_addFromBuffer(toSend, true);
        IAPRetryCounter++;
        return;
    }

    // unlock sending and increase count
    processedExpected = true;
    sequenceToSend++;
    IAPRetryCounter = 0;

    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (NEW_FIRMWARE_ADDRESS + sequenceToSend * 16),
                      (uint32_t) &response[12]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (NEW_FIRMWARE_ADDRESS + sequenceToSend * 16 + 4),
                      (uint32_t) &response[16]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (NEW_FIRMWARE_ADDRESS + sequenceToSend * 16 + 8),
                      (uint32_t) &response[20]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (NEW_FIRMWARE_ADDRESS + sequenceToSend * 16 + 12),
                      (uint32_t) &response[24]);
    HAL_FLASH_Lock();
    return;
}

void IAP_startIAP(uint8_t *sizeBuffer, uint8_t *checksumBuffer) {

    uint32_t size = (sizeBuffer[0] << 24) + (sizeBuffer[1] << 16) + (sizeBuffer[2] << 6) + sizeBuffer[3];
    // uint32_t checksum =
    //(checksumBuffer[0] << 24) + (checksumBuffer[1] << 16) + (checksumBuffer[2] << 6) + checksumBuffer[3];

    IAP_enabled = true;
    EEPROM_WriteByte(FIRMWARE_STATE_ADDRESS, 0x01);
    EEPROM_WriteNBytes(FIRMWARE_SIZE_ADDRESS, sizeBuffer, 4);
    EEPROM_WriteNBytes(FIRMWARE_CHECKSUM_ADDRESS, checksumBuffer, 4);

    uint32_t errorCode;
    uint32_t pages = size / 256 + 1;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef eraseSettings;
    eraseSettings.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseSettings.PageAddress = NEW_FIRMWARE_ADDRESS;
    eraseSettings.NbPages = pages;
    HAL_FLASHEx_Erase(&eraseSettings, &errorCode);

    HAL_FLASH_Lock();

    totalSequences = size / 16;
    if (size % 16 != 0) {
        totalSequences++;
    }

    sequenceToSend = 0;
}
