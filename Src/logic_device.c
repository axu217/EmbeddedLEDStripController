#include "logic_device.h"

uint8_t hardwareVersion;
uint8_t firmwareVersion;
uint8_t category;
uint8_t subcategory;

uint8_t deviceID[4];
uint8_t hubID[4];
bool hubIsLinked = false;

void DEVICE_private_readSavedDeviceData() {
    memcpy(deviceID, (const uint8_t *) IDADDRESS, 4);
    if (deviceID[0] != 0x66) {
        deviceID[0] = 0x66;
        deviceID[1] = 0x01;
        deviceID[2] = 0x00;
        deviceID[3] = 0x01;
    }
    category = deviceID[0];
    subcategory = SUBCATEGORY;
    firmwareVersion = FIRMWAREVERSION;
    hardwareVersion = HARDWAREVERSION;
}

void DEVICE_private_readSavedHubData() {
    LinkData readLink;
    EEPROM_ReadNBytes(HUB_ADDRESS, (uint8_t *) &readLink, 8);
    if ((readLink.ID_HH & 0xf0) == 0x10 && readLink.FlagByte == 0xA3) // has hub linked,and active
    {
        hubIsLinked = true;
        hubID[0] = readLink.ID_HH;
        hubID[1] = readLink.ID_MH;
        hubID[2] = readLink.ID_ML;
        hubID[3] = readLink.ID_LL;
    } else // no hub linked
    {
        hubIsLinked = false; // default HUB for factory test
        hubID[0] = 0x16;
        hubID[1] = 0xfb;
        hubID[2] = 0xfa;
        hubID[3] = 0xfb;
    }
}

void DEVICE_private_readSavedOutletData() {
    uint8_t temp[2];
    EEPROM_ReadNBytes(POWERON_STATUS, temp, 1);
    switch (temp[0]) {
    case 0xAA: // power on resume last status
        EEPROM_ReadNBytes(SWITCH_VALUE, &temp[1], 1);
        if (temp[1] > 1) {
            temp[1] = 0;
        }
        if (temp[1] == 0x01) {
            OUTLET_setShouldBeOn(true);
        } else {
            OUTLET_setShouldBeOn(false);
        }
        break;
    case 0x55: // power on turn on switch
        OUTLET_setShouldBeOn(true);
        break;
    case 0x88: // power on turn off switch
    default:
        OUTLET_setShouldBeOn(false);
        break;
    }
}

void DEVICE_init() {
    DEVICE_private_readSavedHubData();
    DEVICE_private_readSavedOutletData();
    DEVICE_private_readSavedDeviceData();
}

// LINK MANAGER START ---------------------------------------------

bool DEVICE_private_LinkCompare(LinkData *link1, LinkData *link2) {
    if (link1->GroupByte == link2->GroupByte && link1->ID_HH == link2->ID_HH &&
        link1->ID_MH == link2->ID_MH && link1->ID_ML == link2->ID_ML && link1->ID_LL == link2->ID_LL) {
        return true;
    }
    return false;
}

void DEVICE_startLinkingMode() {
    uint8_t data[24];

    uint8_t payload[1];
    payload[0] = 0x00;
    FORMATTER_genericFormat(data, FLAG_broadcast, CMD_link, payload, 1);
    queue_priorityAddFromBuffer(data, false);
}

void DEVICE_endLinkingMode() {
    enum LEDMode temp = Regular;
    LED_setMode(temp);
}

void DEVICE_factoryReset() { EEPROM_FactoryReset(); }

void DEVICE_startUnlinkingMode() {
    uint8_t data[24];
    uint8_t payload[1];
    payload[0] = 0xFF;
    FORMATTER_genericFormat(data, FLAG_broadcast, CMD_link, payload, 1);
    queue_priorityAddFromBuffer(data, false);
}

void DEVICE_endUnlinkingMode() {
    enum LEDMode temp = Regular;
    LED_setMode(temp);
}

bool DEVICE_processMasterLink(LinkData tempLink) {

    uint8_t hh = tempLink.ID_HH;

    if (hh == 0x10 || hh == 0x16) {
        LinkData readLink;
        EEPROM_ReadNBytes(HUB_ADDRESS, (uint8_t *) &readLink, 8);

        if (readLink.FlagByte == 0xA3 && DEVICE_private_LinkCompare(&readLink, &tempLink) == true) {
            DEVICE_endLinkingMode();
            return true;
        }

        EEPROM_WriteNBytes(HUB_ADDRESS, (uint8_t *) &tempLink, 8);
        // REMEMBER TO RESET THE HUB

        DEVICE_endLinkingMode();
        return true;
    } else if (hh == 0x31 || hh == 0x36) {
        LinkData readLink;

        uint8_t earliestEmpty;
        bool tempLock = false;

        for (uint8_t i = 0; i < 31; i++) {
            EEPROM_ReadNBytes(DEVICE_LIST_START_ADDRESS + i * 8, (uint8_t *) &readLink, 8);
            if ((readLink.FlagByte == 0xA3) && DEVICE_private_LinkCompare(&readLink, &tempLink) == true) {
                DEVICE_endLinkingMode();
                return true;
            }

            if ((readLink.FlagByte == 0x00) && tempLock == false) {
                earliestEmpty = i;
                tempLock = true;
            }
        }

        EEPROM_WriteNBytes(earliestEmpty * 8 + DEVICE_LIST_START_ADDRESS, (uint8_t *) &tempLink, 8);

        DEVICE_endLinkingMode();
        return true;
    } else {
        return false;
    }
    return false;
}

bool DEVICE_processMasterUnlink(LinkData tempLink) {

    uint8_t hh = tempLink.ID_HH;
    LinkData readLink;

    if (hh == 0x31 || hh == 0x36) {

        for (uint8_t i = 0; i < 31; i++) {
            EEPROM_ReadNBytes(DEVICE_LIST_START_ADDRESS + i * 8, (uint8_t *) &readLink, 8);

            if ((readLink.FlagByte == 0xA3) && (DEVICE_private_LinkCompare(&readLink, &tempLink) == true)) {
                EEPROM_EraseNBytes(DEVICE_LIST_START_ADDRESS + i * 8, 8);
                DEVICE_endUnlinkingMode();
                return true;
            }
        }
        return false;

    } else if (hh == 0x10 || hh == 0x16) {

        EEPROM_ReadNBytes(HUB_ADDRESS, (uint8_t *) &readLink, 8);

        if ((readLink.FlagByte == 0xA3) && (DEVICE_private_LinkCompare(&readLink, &tempLink) == true)) {
            EEPROM_EraseNBytes(HUB_ADDRESS, 8);
            DEVICE_endUnlinkingMode();
            return true;
        }
        return false;

    } else {
        return false;
    }
    return false;
}

// LINK MANAGER END ---------------------------------------------

//

// PUBLIC GETTERS START ----------------------------------------

uint8_t DEVICE_getHardwareVersion() { return hardwareVersion; }

uint8_t DEVICE_getSoftwareVersion() { return firmwareVersion; }

uint8_t DEVICE_getSubcategory() { return subcategory; }

uint8_t DEVICE_getCategory() { return category; }

uint8_t *DEVICE_getID() { return deviceID; }

uint8_t *DEVICE_getHubID() { return hubID; }

bool DEVICE_getHubIsLinked() { return hubIsLinked; }
