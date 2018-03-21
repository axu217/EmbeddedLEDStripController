#ifndef __UTIL_constant__
#define __UTIL_constant__

// Broadcast Commands
#define CMD_link 0x0008
#define CMD_powerOn 0xFFFF
#define CMD_RFTest 0xFFFE

// Direct Commands
#define CMD_startIAP 0xF0
#define CMD_unlink 0x0009
#define CMD_factoryReset 0x0004
#define CMD_setPoweronStatus 0x000B
#define CMD_getDeviceStatus 0x0017
#define CMD_setDeviceStatus 0x001A
#define CMD_setLoraSettings 0x0030
#define CMD_setSleepMinutes 0x001D
#define CMD_setWakeupMinutes 0x001E
#define CMD_getSocketSchedule 0x0022
#define CMD_setSocketSchedule 0x0023
#define CMD_getDeviceInformation 0x0016

#define CMD_setSlaveTime 0x0080

// Acknowledge Commands
#define CMD_grantFirmwareData 0xF1

// ETC

#define GRP_ID_MASK 0xA0
#define MAX_NUM_SCHEDULES 0x06

#endif