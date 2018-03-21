#ifndef __LOGIC_DEVICE__
#define __LOGIC_DEVICE__

#include "UTIL_BASE.h"
#include "main.h"
#include "module_led.h"
#include "module_outlet.h"
#include "subLogic_formatter.h"
#include "subLogic_queue.h"

// link data
typedef struct {
    unsigned char FlagByte;
    unsigned char GroupByte;
    unsigned char ID_HH;
    unsigned char ID_MH;
    unsigned char ID_ML;
    unsigned char ID_LL;
    unsigned char Dat1;
    unsigned char Dat2;
} LinkData;

typedef enum { MODE_SLAVE = 0x00, MODE_MASTER = 0x01, MODE_EACH = 0x03, MODE_DELETE = 0xff } LinkMode;

// Constants
static const uint8_t SUBCATEGORY = 0x01;
static const uint8_t FIRMWAREVERSION = 0x09;
static const uint8_t HARDWAREVERSION = 0x01;

void DEVICE_init();
uint8_t *DEVICE_getID();
uint8_t *DEVICE_getHubID();
uint8_t DEVICE_getHardwareVersion();
uint8_t DEVICE_getSoftwareVersion();
uint8_t DEVICE_getSubcategory();
uint8_t DEVICE_getCategory();
bool DEVICE_getHubIsLinked();

void DEVICE_startLinkingMode();

void DEVICE_endLinkingMode();

void DEVICE_startUnlinkingMode();

void DEVICE_endUnlinkingMode();

bool DEVICE_processMasterLink(LinkData tempLink);

bool DEVICE_processMasterUnlink(LinkData tempLink);

void DEVICE_factoryReset();

#endif