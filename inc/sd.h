#ifndef _BEAGLE_SD_H
#define _BEAGLE_SD_H

#include <stddef.h>
#include <stdint.h>

#define SD_BLKLEN 0x200

extern int sd_init(void);
extern int sd_read(void *, uint32_t);
extern int sd_reads(void *, uint32_t, size_t);

#endif /* _BEAGLE_SD_H */
