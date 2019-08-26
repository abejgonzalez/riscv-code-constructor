#ifndef _BEAGLE_COMMON_H
#define _BEAGLE_COMMON_H

#define likely(x)       __builtin_expect ((x), 1)
#define unlikely(x)     __builtin_expect ((x), 0)

static inline void relax(void)
{
	long dummy;
	/* Induce long-latency stall */
	__asm__ __volatile__ ("div %0, %0, zero" : "=r" (dummy));
}

extern void __attribute__((noreturn)) hang(void);

enum status {
	OK = 0,
	ERROR_SD_CMD0,
	ERROR_SD_CMD8,
	ERROR_SD_ACMD41,
	ERROR_SD_CMD58,
	ERROR_SD_CMD16,
	ERROR_SD_CMD17,
	ERROR_SD_CMD18,
	ERROR_SD_DATA_CRC,
	ERROR_GPT_SIGNATURE,
	ERROR_GPT_ENTRY_SIZE,
	ERROR_GPT_ENTRY_ABSENT,
};


#endif /* _BEAGLE_COMMON_H */
