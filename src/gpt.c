#include <stddef.h>
#include <stdint.h>

#include "gpt.h"
#include "sd.h"
#include "tty.h"
#include "common.h"

#if (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__)
#error "Expected little-endian byte order"
#endif

/* 4fab7a49-4c26-49a4-92cf-9128078d8a17 */
const struct gpt_guid gpt_type_eagle = {
	.time_low = 0x4FAB7A49,
	.time_mid = 0x4C26,
	.time_hi_and_version = 0x49A4,
	.clock_seq_hi = 0x92,
	.clock_seq_low = 0xcf,
	.node = { 0x91, 0x28, 0x07, 0x8d, 0x8a, 0x17 },
};

static inline int gpt_guid_equal(
	const struct gpt_guid *a, const struct gpt_guid *b)
{
	const uint64_t *ap = (const uint64_t *)a;
	const uint64_t *bp = (const uint64_t *)b;
	const uint64_t *end = (const uint64_t *)(a + 1);
	do {
		if (*ap++ != *bp++) {
			return 0;
		}
	} while (ap < end);
	return 1;
}

int gpt_load(const struct gpt_guid *type, void *dest)
{
	uint8_t buf[SD_BLKLEN];
	uint64_t lba;
	uint32_t size, count;
	size_t stride, offset;
	int rc;

	put_s("boot: reading GPT header\r\n");
	if ((rc = sd_read(&buf, GPT_HEADER_LBA))) {
		return rc;
	}
	{
		const struct gpt_header *hdr;
		hdr = (const struct gpt_header *)buf;
		if (hdr->signature != GPT_HEADER_SIGNATURE) {
			return ERROR_GPT_SIGNATURE;
		}
		lba = hdr->partition_array_lba;
		count = hdr->partition_array_len;
		size = hdr->partition_entry_size;
	}
	/* Entry size expected to be a multiple of 128 bytes */
	if ((size < sizeof(struct gpt_entry)) || (size & (size - 1)) != 0) {
		return ERROR_GPT_ENTRY_SIZE;
	}
	/* Convince the compiler that offset is 64-bit aligned */
	size &= ~(sizeof(struct gpt_entry) - 1);

	stride = (size + sizeof(buf) - 1) / sizeof(buf);
	for (offset = 0; count > 0; offset += size, count--) {
		const struct gpt_entry *entry;
		if (offset >= sizeof(buf)) {
			offset = 0;
			if ((rc = sd_read(&buf, lba))) {
				return rc;
			}
			lba += stride;
		}
		entry = (const struct gpt_entry *)(buf + offset);
		if (gpt_guid_equal(&(entry->type), type)) {
			put_s("boot: loading GPT partition [LBA 0x");
			put_x(entry->lba_start);
			put_s("-0x");
			put_x(entry->lba_end);
			put_s("]\r\n");
			return sd_reads(dest, entry->lba_start,
				entry->lba_end - entry->lba_start + 1);
		}
	}
	put_s("boot: no valid GPT partition\r\n");
	return ERROR_GPT_ENTRY_ABSENT;
}
