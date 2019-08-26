#ifndef _BEAGLE_GPT_H
#define _BEAGLE_GPT_H

#define GPT_HEADER_LBA          1
#define GPT_HEADER_SIGNATURE    (0x5452415020494645ULL)

/* NOTE: Little-endian */
struct gpt_guid {
	uint32_t time_low;
	uint16_t time_mid;
	uint16_t time_hi_and_version;
	uint8_t clock_seq_hi;
	uint8_t clock_seq_low;
	uint8_t node[6];
} __attribute__ ((packed))
  __attribute__ ((aligned(sizeof(uint64_t))));

struct gpt_entry {
	struct gpt_guid type;
	struct gpt_guid partition_guid;
	uint64_t lba_start;
	uint64_t lba_end;
	uint64_t attrs;
	uint16_t name[36];
} __attribute__ ((packed));

struct gpt_header {
	uint64_t signature;
	uint32_t revision;
	uint32_t header_size;
	uint32_t header_crc;
	uint32_t _reserved1;
	uint64_t current_lba;
	uint64_t backup_lba;
	uint64_t first_usable_lba;
	uint64_t last_usable_lba;
	struct gpt_guid disk_guid;
	uint64_t partition_array_lba;
	uint32_t partition_array_len;
	uint32_t partition_entry_size;
	uint32_t partition_array_crc;
	uint8_t _reserved2[512 - 92];
} __attribute__ ((packed));

extern const struct gpt_guid gpt_type_eagle;

extern int gpt_load(const struct gpt_guid *, void *);

#endif /* _EABLE_GPT_H */
