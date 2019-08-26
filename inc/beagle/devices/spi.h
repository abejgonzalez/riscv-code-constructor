#ifndef _BEAGLE_SPI_H
#define _BEAGLE_SPI_H

#include <beagle/const.h>

#ifndef __ASSEMBLY__
#include <stdint.h>

typedef volatile struct spi {
	uint32_t sckdiv;
	uint32_t sckmode;
	uint64_t __reserved_0x08;
	uint32_t csid;
	uint32_t csdef;
	uint32_t csmode;
	uint32_t __reserved_0x1c;

	uint64_t __reserved_0x20;
	uint16_t dcssck;
	uint16_t dsckcs;
	uint16_t dintercs;
	uint16_t dinterxfr;

	uint64_t __reserved_0x30;
	uint64_t __reserved_0x38;
	uint16_t fmt;
	uint16_t len;
	uint32_t __reserved_0x44;
	uint32_t txfifo;
	uint32_t rxfifo;
	uint32_t txmark;
	uint32_t rxmark;
	uint64_t __reserved_0x58;

	uint32_t insnmode;
	uint32_t insnfmt;
	uint64_t __reserved_0x68;

	uint32_t ie;
	uint32_t ip;
} __attribute__ ((packed)) spi_t;

#endif /* !__ASSEMBLY__ */

#define SPI_ADDR                _AC(0xA000,UL)

#define SPI_DIVISOR(ref, sck)   ((ref) / (((sck) << 1) - 1))

#define SPI_CSMODE_AUTO         0
#define SPI_CSMODE_HOLD         2
#define SPI_CSMODE_OFF          3

#endif /* _BEAGLE_SPI_H */
