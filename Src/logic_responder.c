#include "logic_responder.h"

extern RTC_HandleTypeDef hrtc;

bool RESPONDER_private_filterID(uint8_t *inputRxBuffer) {

    uint8_t *myID = DEVICE_getID();

    if (inputRxBuffer[4] == myID[0] && inputRxBuffer[5] == myID[1] && inputRxBuffer[6] == myID[2] &&
        inputRxBuffer[7] == myID[3]) {
        return true;
    }

    return false;
}

bool RESPONDER_private_filterHubID(uint8_t *inputRxBuffer) {
    uint8_t *hubID = DEVICE_getHubID();

    if (inputRxBuffer[0] == hubID[0] && inputRxBuffer[1] == hubID[1] && inputRxBuffer[2] == hubID[2] &&
        inputRxBuffer[3] == hubID[3] && (DEVICE_getHubIsLinked() == true)) {
        return true;
    }

    return false;
}

void RESPONDER_handleMessage() {
    uint8_t rxBuffer[28];
    SX1278LoRaReadRxBuffer(rxBuffer);

    uint8_t flag = rxBuffer[8];
    switch (flag) {
    case FLAG_broadcast:
        RESPONDER_private_handleBroadcast(rxBuffer);
        break;
    case FLAG_direct:
        if (RESPONDER_private_filterID(rxBuffer) == false) {
            return;
        }
        RESPONDER_private_handleDirect(rxBuffer);
        break;
    case FLAG_acknowledge:
        RESPONDER_private_handleDirectAck(rxBuffer);
        break;
    default:
        break;
    }
}

void RESPONDER_private_handleBroadcast(uint8_t *rxBuffer) {

    uint8_t command1 = rxBuffer[9];
    uint8_t command2 = rxBuffer[10];
    uint16_t totalCommand = (command1 << 8) + command2;

    if (totalCommand == CMD_link) {
        // Device should senses the broadcast link. What does it do?
        return;
    }

    // Done
    if (totalCommand == CMD_setSlaveTime) {

        if (RESPONDER_private_filterHubID(rxBuffer) == false) {
            return;
        }

        uint8_t second = RFRxBuffer[11];
        uint8_t minute = RFRxBuffer[12];
        uint8_t hour = RFRxBuffer[13];
        uint8_t day = RFRxBuffer[14];
        uint8_t month = RFRxBuffer[15];
        uint8_t year = RFRxBuffer[16];

        SunTime sunrise;
        SunTime sunset;

        sunrise.hour = RFRxBuffer[17];
        sunrise.minute = RFRxBuffer[18];
        sunset.hour = RFRxBuffer[19];
        sunset.minute = RFRxBuffer[20];

        SCHEDULER_setSunset(sunset);
        SCHEDULER_setSunrise(sunrise);

        RTC_DateTypeDef dayTypeDef;
        dayTypeDef.WeekDay = (uint8_t) TIMER_getWeekDay(year, month, day);
        dayTypeDef.Month = month;
        dayTypeDef.Date = day;
        dayTypeDef.Year = year;

        RTC_TimeTypeDef newTime;
        newTime.Hours = hour;
        newTime.Minutes = minute;
        newTime.Seconds = second;
        newTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        newTime.StoreOperation = RTC_STOREOPERATION_RESET;

        HAL_RTC_SetDate(&hrtc, &dayTypeDef, RTC_FORMAT_BIN);
        HAL_RTC_SetTime(&hrtc, &newTime, RTC_FORMAT_BIN);
        return;
    }
}

void RESPONDER_private_handleDirect(uint8_t *rxBuffer) {

    uint8_t command1 = rxBuffer[9];
    uint8_t command2 = rxBuffer[10];
    uint16_t totalCommand = (command1 << 8) + command2;

    // DONE
    if (totalCommand == CMD_link) {

        bool success;
        // IF LINKING IS GOOD SEND BACK AN ACKNOWLEDGEMENT. ELSE DON'T.
        LinkData tempLink;
        tempLink.GroupByte = rxBuffer[12];
        tempLink.ID_HH = rxBuffer[0];
        tempLink.ID_MH = rxBuffer[1];
        tempLink.ID_ML = rxBuffer[2];
        tempLink.ID_LL = rxBuffer[3];
        tempLink.Dat1 = 1;
        tempLink.Dat2 = 0;
        switch (rxBuffer[11]) {

        case MODE_MASTER:
            // what is this code here?
            tempLink.FlagByte = GRP_ID_MASK | 0x03;
            success = DEVICE_processMasterLink(tempLink);

            if (success == true) {
                uint8_t data[24];
                uint8_t payload[1];
                // payload[0] == 0x00;
                FORMATTER_genericFormat(data, FLAG_acknowledge, CMD_link, payload, 1);
                queue_priorityAddFromBuffer(data, true);
            }
            break;
        case MODE_DELETE:

            success = DEVICE_processMasterUnlink(tempLink);
            if (success) {
                uint8_t data[24];
                uint8_t payload[1];

                payload[0] = 0xFF;

                FORMATTER_genericFormat(data, FLAG_acknowledge, CMD_link, payload, 1);
                queue_priorityAddFromBuffer(data, false);
            }
            break;
        default:
            break;
        }
        return;
    }

    // MAKES SURE MESSAGE COMES FROM LINKED HUB. DOESN'T KNOW LINKED AT SOMEPOINT SO CHECK AFTER LINK PROCESS
    if (RESPONDER_private_filterHubID(rxBuffer) == false) {
        return;
    }

    // DONE
    if (totalCommand == CMD_unlink) {
        if (rxBuffer[12] == 0xff && rxBuffer[11] == 0xff) {
            uint8_t data[28];
            uint8_t payload[4];
            payload[0] = rxBuffer[11];
            payload[1] = rxBuffer[12];
            payload[2] = 0xAA;
            payload[3] = 0xAA;

            FORMATTER_genericFormat(data, FLAG_acknowledge, CMD_unlink, payload, 4);
            queue_priorityAddFromBuffer(data, false);

            // HUB RESET
            EEPROM_EraseNBytes(HUB_ADDRESS, 8);
        }
    }

    // DONE
    if (totalCommand == CMD_factoryReset) {

        if (rxBuffer[12] == 0xff && rxBuffer[11] == 0xff) {
            uint8_t data[28];
            uint8_t payload[4];
            payload[0] = rxBuffer[11];
            payload[1] = rxBuffer[12];
            payload[2] = 0xAA;
            payload[3] = 0xAA;

            FORMATTER_genericFormat(data, FLAG_acknowledge, CMD_factoryReset, payload, 4);
            queue_priorityAddFromBuffer(data, false);

            // Factory Reset
            DEVICE_factoryReset();
        }
    }

    // DONE
    if (totalCommand == CMD_setPoweronStatus) {
        uint8_t data[24];
        if (rxBuffer[11] == 0x05) {
            uint8_t ByteCheckSum = 0;
            switch (rxBuffer[12]) {
            case 0x55:
            case 0x88:
            case 0xaa: // write
                EEPROM_ReadNBytes(POWERON_STATUS, &ByteCheckSum, 1);
                if (ByteCheckSum != rxBuffer[12]) {
                    EEPROM_WriteNBytes(POWERON_STATUS, &rxBuffer[12], 1);
                }

                FORMATTER_genericFormat(data, FLAG_acknowledge, CMD_setPoweronStatus, &rxBuffer[11], 2);
                queue_priorityAddFromBuffer(data, false);
                break;
            case 0x00: // read
                EEPROM_ReadNBytes(POWERON_STATUS, &ByteCheckSum, 1);
                if (ByteCheckSum != 0x55 && ByteCheckSum != 0x88 && ByteCheckSum != 0xAA) {
                    ByteCheckSum = 0x88;
                }
                uint8_t payload[2];

                payload[0] = rxBuffer[11];
                payload[1] = ByteCheckSum;

                FORMATTER_genericFormat(data, FLAG_acknowledge, CMD_setPoweronStatus, payload, 2);
                queue_priorityAddFromBuffer(data, false);
                break;
            }
        }
        return;
    }

    // DONE
    if (totalCommand == CMD_getDeviceStatus) {

        uint8_t formatPacket[24];
        uint8_t data[16];

        bool temp = OUTLET_getShouldBeOn();
        if (temp == true) {
            data[0] = 0x01;
        }
        data[1] = 1;
        data[2] = SCHEDULER_getSleepMinutes() / 256;
        data[3] = SCHEDULER_getSleepMinutes() % 256;
        data[4] = SCHEDULER_getWakeupMinutes() / 256;
        data[5] = SCHEDULER_getWakeupMinutes() % 256;
        data[6] = 0;
        data[7] = 0;

        SunTime sunrise = SCHEDULER_getSunrise();
        SunTime sunset = SCHEDULER_getSunset();
        data[8] = sunrise.hour;
        data[9] = sunrise.minute;
        data[10] = sunset.hour;
        data[11] = sunset.minute;

        RTC_TimeTypeDef tempTime;
        HAL_RTC_GetTime(&hrtc, &tempTime, RTC_FORMAT_BIN);
        RTC_DateTypeDef tempDay;
        HAL_RTC_GetDate(&hrtc, &tempDay, RTC_FORMAT_BIN);

        data[12] = tempDay.Year;
        data[13] = tempDay.Month;
        data[14] = tempDay.Date;
        data[15] = tempTime.Hours;
        data[16] = tempTime.Minutes;

        FORMATTER_genericFormat(formatPacket, FLAG_acknowledge, CMD_getDeviceStatus, data, 17);
        queue_priorityAddFromBuffer(formatPacket, false);
    }

    // DONE
    if (totalCommand == CMD_setDeviceStatus) {
        uint8_t result = rxBuffer[11];
        if (result == 0x01) {
            OUTLET_setShouldBeOn(true);
        } else {
            OUTLET_setShouldBeOn(false);
        }
    }

    // DONE
    if (totalCommand == CMD_setSleepMinutes) {
        if (rxBuffer[11] != 1) {
            return;
        }
        SCHEDULER_setSleepMinutes(rxBuffer[12] * 256 + rxBuffer[13]);

        uint8_t data[24];
        FORMATTER_genericFormat(data, FLAG_acknowledge, CMD_setSleepMinutes, &rxBuffer[11], 3);
        queue_addFromBuffer(data, false);
    }

    // DONE
    if (totalCommand == CMD_setWakeupMinutes) {
        if (rxBuffer[11] != 1) {
            return;
        }
        SCHEDULER_setWakeupMiniutes(rxBuffer[12] * 256 + rxBuffer[13]);

        uint8_t data[24];
        FORMATTER_genericFormat(data, FLAG_acknowledge, CMD_setWakeupMinutes, &rxBuffer[11], 3);
        queue_addFromBuffer(data, false);
    }

    // DONE
    if (totalCommand == CMD_getSocketSchedule) {

        // magic number 10

        if (rxBuffer[11] == 0 || rxBuffer[11] > MAX_NUM_SCHEDULES) {
            return;
        }
        uint8_t data[14];

        data[0] = rxBuffer[11];
        EEPROM_ReadNBytes(SCHEDULE_BASE + (rxBuffer[11] - 1) * 6, &data[1], 6);
        data[7] = rxBuffer[11] + 1;
        EEPROM_ReadNBytes(SCHEDULE_BASE + rxBuffer[11] * 6, &data[8], 6);

        uint8_t packet[24];
        FORMATTER_genericFormat(packet, FLAG_acknowledge, CMD_getSocketSchedule, data, 14);
        queue_priorityAddFromBuffer(packet, false);
        return;
    }

    // NOT DONE
    if (totalCommand == CMD_setSocketSchedule) {

        // MAGIC NUMBER 10
        if (rxBuffer[11] % 2 == 0 || rxBuffer[11] > MAX_NUM_SCHEDULES) {
            return;
        }

        EEPROM_WriteNBytes(SCHEDULE_BASE + (rxBuffer[11] - 1) * 6, &rxBuffer[12], 6);

        uint8_t data[7];
        data[0] = rxBuffer[11];
        EEPROM_ReadNBytes(SCHEDULE_BASE + (rxBuffer[11] - 1) * 6, &data[1], 6);

        uint8_t packet[24];

        FORMATTER_genericFormat(packet, FLAG_acknowledge, CMD_setSocketSchedule, data, 8);
        queue_priorityAddFromBuffer(packet, false);
        return;
    }

    // DONE
    if (totalCommand == CMD_getDeviceInformation) {
        // Reply
        uint8_t buffer[28];
        uint8_t payload[4];
        payload[0] = DEVICE_getCategory();
        payload[1] = DEVICE_getSubcategory();
        payload[2] = DEVICE_getSoftwareVersion();
        payload[3] = DEVICE_getHardwareVersion();
        FORMATTER_genericFormat(buffer, FLAG_acknowledge, CMD_getDeviceInformation, payload, 4);
        queue_priorityAddFromBuffer(buffer, false);
    }

    // DONE
    if (command1 == CMD_startIAP) {
        uint8_t data[28];
        memcpy(data, DEVICE_getID(), 4);
        memcpy(&data[4], DEVICE_getHubID(), 4);
        data[8] = 0x20;
        data[9] = 0xF1;
        queue_priorityAddFromBuffer(data, false);

        IAP_startIAP(&rxBuffer[11], &rxBuffer[15]);
    }
}

void RESPONDER_private_handleDirectAck(uint8_t *rxBuffer) {
    uint8_t command1 = rxBuffer[9];
    // uint8_t command2 = rxBuffer[10];

    if (command1 == 0xF1) {
        IAP_process(rxBuffer);
    }
}
