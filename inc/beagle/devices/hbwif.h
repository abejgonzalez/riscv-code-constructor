#ifndef _BEAGLE_HBWIF_H
#define _BEAGLE_HBWIF_H

#include <beagle/const.h>

#ifndef __ASSEMBLY__
#include <stdint.h>

typedef volatile struct hbwif {
	uint64_t rx_invert;
	uint64_t tx_invert;
	uint64_t decoder_clear_error;
	uint64_t decoder_error;
	uint64_t mem_mode_enable;
	uint64_t bit_stuffer_mode;

	uint64_t pattern_mem_pattern[16];
	uint64_t pattern_mem_pattern_enable;
	uint64_t pattern_mem_snapshot_enable;
	uint64_t pattern_mem_snapshot[16];
	uint64_t pattern_mem_snapshot_valid;

	uint64_t bert_ber_mode;
	uint64_t bert_clear;
	uint64_t bert_enable;
	uint64_t bert_prbs_load;
	uint64_t bert_prbs_mode_rx;
	uint64_t bert_prbs_mode_tx;
	uint64_t bert_prbs_select;
	uint64_t bert_sample_count;

	uint64_t bert_error_counts[2];
	uint64_t bert_prbs_seed_goods;
	uint64_t bert_sample_count_out;
} __attribute__ ((packed)) hbwif_t;

#endif /* !__ASSEMBLY__ */

#define _HBWIF_CONFIG_ADDR      _AC(0x4000000,UL)
#define HBWIF_CONFIG_ADDR(i)    _AT(hbwif_t *, _HBWIF_CONFIG_ADDR + ((i) << 16))
#define HBWIF_NLANES            2

#endif /* _BEAGLE_HBWIF_H */
