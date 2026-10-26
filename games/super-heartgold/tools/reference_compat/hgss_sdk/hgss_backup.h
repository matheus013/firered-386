/* HGSS infrared-cart backup variant. Verified against SS USA ARM9:
 * SaveDetectFlash literal at 0x02028750 = 0x00ff1302.
 * Vendor 0xff is CARD_BACKUP_TYPE_VENDER_IRC in libntr card_spi.c.
 * This declaration does not implement or validate persistent storage.
 */
#ifndef HGSS_BACKUP_COMPAT_H
#define HGSS_BACKUP_COMPAT_H
#include <nitro/card/common.h>
#define CARD_BACKUP_TYPE_FLASH_4MBITS_EX \
    ((CARDBackupType)CARD_BACKUP_TYPE_DEFINE(FLASH, 19, 255))
#endif
