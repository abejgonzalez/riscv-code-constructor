#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <beagle/devices/spi.h>

#include "sd.h"
#include "tty.h"
#include "common.h"

/* Prepend start bit (0) and transmission bit (1) */
#define SD_CMD(idx)     (0x40 | ((idx) & 0x3F))

#define SD_CMD_GO_IDLE_STATE	        SD_CMD(0)
#define SD_CMD_SEND_IF_COND	        SD_CMD(8)
#define SD_CMD_STOP_TRANSMISSION        SD_CMD(12)
#define SD_CMD_SET_BLOCKLEN             SD_CMD(16)
#define SD_CMD_READ_SINGLE_BLOCK        SD_CMD(17)
#define SD_CMD_READ_MULTIPLE_BLOCK      SD_CMD(18)
#define SD_CMD_APP_CMD                  SD_CMD(55)
#define SD_CMD_APP_SEND_OP_COND         SD_CMD(41)
#define SD_CMD_READ_OCR                 SD_CMD(58)

#define SD_TOKEN_READY                  0x00 /* R1 response */
#define SD_TOKEN_IDLE                   0x01 /* R1 response */
#define SD_TOKEN_START_BLOCK            0xFE

#define SD_FREQ_OD      300000UL
#define SD_FREQ_PP      25000000UL


static spi_t * const spi = (spi_t *)(SPI_ADDR);

static inline void spi_tx(uint8_t d)
{
#if __riscv_atomic
	int32_t r;
	do {
		__asm__ __volatile__ (
			"amoor.w %0, %2, %1\n"
			: "=r" (r), "+A" (spi->txfifo)
			: "r" ((uint32_t)d));
	} while (r < 0);
#else
	while ((int32_t)spi->txfifo < 0);
	spi->txfifo = d;
#endif
}

static inline uint8_t spi_rx(void)
{
	int32_t r;
	while ((r = spi->rxfifo) < 0) {
		relax();
	}
	return r;
}

static inline uint8_t spi_txrx(uint8_t d)
{
	spi_tx(d);
	return spi_rx();
}

static inline uint8_t sd_dummy(void)
{
	return spi_txrx(0xFF);
}


static uint8_t sd_cmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	unsigned long n;
	uint8_t r;

	spi->csmode = SPI_CSMODE_HOLD;
	sd_dummy();
	spi_txrx(cmd);
	spi_txrx(arg >> 24);
	spi_txrx(arg >> 16);
	spi_txrx(arg >> 8);
	spi_txrx(arg);
	spi_txrx(crc);

	n = 1000;
	do {
		r = sd_dummy();
		/* Zero MSB indicates response token */
		if (!(r & 0x80)) {
			break;
		}
	} while (--n > 0);
	printf("sd_cmd: resp:%d (cmd,arg,crc):(0x%x,0x%x,0x%x)\n", r, cmd, arg, crc);
	return r;
}

static inline void sd_cmd_end(void)
{
	/* <https://stackoverflow.com/questions/2365897/initializing-sd-card-in-spi-issues> */
	sd_dummy();
	spi->csmode = SPI_CSMODE_AUTO;
}


/* Power supply ramp-up sequence */
static inline void sd_poweron(void)
{
	unsigned long i;

	spi->sckdiv = SPI_DIVISOR(REFCLK, SD_FREQ_OD);
	/*  Wait 1ms for supply voltage stabilization */
        /*  Issue 74+ clock pulses while setting CMD high */
	spi->csmode = SPI_CSMODE_OFF;
	for (i = 50; i > 0; i--) {
		sd_dummy();
	}
	spi->csmode = SPI_CSMODE_AUTO;
}

static inline int sd_cmd0(void)
{
	int rc;
	rc = (sd_cmd(SD_CMD_GO_IDLE_STATE, 0, 0x95) != SD_TOKEN_IDLE);
	sd_cmd_end();
	return rc;
}

static inline int sd_cmd8(void)
{
	int rc;
	rc = (sd_cmd(SD_CMD_SEND_IF_COND, 0x000001AA, 0x87) != SD_TOKEN_IDLE);
	sd_dummy(); /* command version; reserved */
	sd_dummy(); /* reserved */
	rc |= ((sd_dummy() & 0xF) != 0x1); /* voltage */
	rc |= (sd_dummy() != 0xAA); /* check pattern */
	sd_cmd_end();
	return rc;
}

static inline void sd_cmd55(void)
{
	sd_cmd(SD_CMD_APP_CMD, 0, 0x65);
	sd_cmd_end();
}

static inline int sd_acmd41(void)
{
	uint8_t r;
	do {
		sd_cmd55();
		r = sd_cmd(SD_CMD_APP_SEND_OP_COND, 0x40000000, 0x77); /* HCS = 1 */
	} while (r == SD_TOKEN_IDLE);
	return (r != SD_TOKEN_READY);
}

static inline int sd_cmd58(void)
{
	int rc;
	rc = (sd_cmd(SD_CMD_READ_OCR, 0, 0xFD) != SD_TOKEN_READY);
	rc |= ((sd_dummy() & 0x80) != 0x80); /* Power up status */
	sd_dummy();
	sd_dummy();
	sd_dummy();
	sd_cmd_end();
	return rc;
}

static inline int sd_cmd16(void)
{
	int rc;
	rc = (sd_cmd(SD_CMD_SET_BLOCKLEN, SD_BLKLEN, 0x15) != SD_TOKEN_READY);
	sd_cmd_end();
	return rc;
}

int sd_init(void)
{
	put_s("boot: initializing SD card\r\n");

	sd_poweron();
	if (sd_cmd0())   { return ERROR_SD_CMD0; }
	if (sd_cmd8())   { return ERROR_SD_CMD8; }
	if (sd_acmd41()) { return ERROR_SD_ACMD41; }
//	if (sd_cmd58())  { return ERROR_SD_CMD58; }
	if (sd_cmd16())  { return ERROR_SD_CMD16; }

	spi->sckdiv = SPI_DIVISOR(REFCLK, SD_FREQ_PP);
	return OK;
}

static inline uint8_t crc7(uint8_t crc, uint8_t in)
{
	/* Polynomial: x^7 + x^3 + 1 */
	uint8_t k = (crc << 1) ^ in;
	return k ^ (k >> 7) ^
		((crc >> 3) & 0x7) ^
		(crc & 0x78) ^
		(crc << 4) ^
		((in >> 4) & 0x8);
}

static uint8_t sd_cmd_crc(uint8_t cmd, uint32_t arg)
{
	uint8_t crc;
	crc = crc7(0, cmd);
	crc = crc7(crc, arg >> 24);
	crc = crc7(crc, arg >> 16);
	crc = crc7(crc, arg >> 8);
	crc = crc7(crc, arg);
	crc &= 0x7f;
	/* Augmentation: shift in 7 more zero bits */
	crc = crc ^ (crc >> 4) ^ (crc << 3) ^ ((crc >> 1) & 0x38);

	return sd_cmd(cmd, arg, (crc << 1) | 0x1);
}

static inline uint16_t crc16(uint16_t crc, uint8_t in)
{
	/* Polynomial: x^16 + x^12 + x^5 + 1 */
	crc = (uint8_t)(crc >> 8) | (crc << 8);
	crc ^= in;
	crc ^= (uint8_t)(crc >> 4) & 0xf;
	crc ^= crc << 12;
	crc ^= (crc & 0xff) << 5;
	return crc;
}

static int sd_data(uint8_t *buf)
{
	uint16_t crc, crc_exp;
	uint8_t *end;

	while (sd_dummy() != SD_TOKEN_START_BLOCK);
	crc = 0;
	end = buf + SD_BLKLEN;
	do {
		uint8_t x = sd_dummy();
		*buf++ = x;
		crc = crc16(crc, x);
	} while (buf < end);

	crc_exp = ((uint16_t)sd_dummy() << 8);
	crc_exp |= sd_dummy();
	printf("crc = %d, crc_exp = %d\n", crc, crc_exp);
	return (crc != crc_exp) ? ERROR_SD_DATA_CRC : 0;
}

int sd_read(void *buf, uint32_t addr)
{
	int rc;
	rc = ERROR_SD_CMD17;
	if (sd_cmd_crc(SD_CMD_READ_SINGLE_BLOCK, addr) == SD_TOKEN_READY) {
		rc = sd_data(buf);
	}
	sd_cmd_end();
	return rc;
}

int sd_reads(void *buf, uint32_t addr, size_t n)
{
	int rc;
	rc = ERROR_SD_CMD18;
	if (sd_cmd_crc(SD_CMD_READ_MULTIPLE_BLOCK, addr) == SD_TOKEN_READY) {
		uint8_t *p;
		for (p = buf; n > 0; n--, p += SD_BLKLEN) {
			if ((rc = sd_data(p))) {
				break;
			}
		}
		sd_cmd(SD_CMD_STOP_TRANSMISSION, 0, 0x61);
	}
	sd_cmd_end();
	return rc;
}
