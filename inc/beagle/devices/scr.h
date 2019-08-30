#ifndef _BEAGLE_SCR_H
#define _BEAGLE_SCR_H

#include <beagle/const.h>

#ifndef __ASSEMBLY__
#include <stddef.h>
#include <stdint.h>

typedef volatile struct scr {
    uint32_t boot;

	uint32_t switcher_sel;

	uint32_t hbwif_resets;
	uint32_t bh_reset;
	uint32_t rs_reset;

	uint32_t __reserved_0x14;
	uint32_t __reserved_0x18;
	uint32_t __reserved_0x1C;

	uint32_t uncore_clk_div;
	uint32_t bh_clk_div;
	uint32_t rs_clk_div;
	uint32_t bh_clk_out_div;
	uint32_t rs_clk_out_div;
	uint32_t lbwif_clk_div;

	uint32_t __reserved_0x38;
	uint32_t __reserved_0x3C;
	uint32_t __reserved_0x40;
	uint32_t __reserved_0x44;
	uint32_t __reserved_0x48;
	uint32_t __reserved_0x4C;

	uint32_t uncore_clk_pass_sel;
	uint32_t bh_clk_pass_sel;
	uint32_t rs_clk_pass_sel;
	uint32_t lbwif_clk_pass_sel;
} __attribute__ ((packed)) scr_t;

#endif /* !__ASSEMBLY__ */

#define SCR_ADDR        _AC(0x110000,UL)

#define SCR_REG_UNCORE_CLK_PASS_SEL    0x50
_ASSERT_OFFSETOF(scr_t, uncore_clk_pass_sel, SCR_REG_UNCORE_CLK_PASS_SEL)

#define SCR_REG_BOOT    0x00
_ASSERT_OFFSETOF(scr_t, boot, SCR_REG_BOOT)

#define SWITCHER_SEL_HBWIF      0
#define SWITCHER_SEL_LBWIF      1

#endif /* _BEAGLE_SCR_H */
