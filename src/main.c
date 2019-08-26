#include <beagle/devices/scr.h>
#include <beagle/devices/hbwif.h>
#include <beagle/devices/uart.h>
#include <beagle/sections.h>
#include <beagle/csr.h>

#include "sd.h"
#include "gpt.h"
#include "tty.h"
#include "common.h"
#include "boot.h"

static scr_t * const scr = (scr_t *)(SCR_ADDR);
static uart_t * const uart = (uart_t *)(UART_ADDR);

void handle_trap(void)
{
#ifdef DEBUG
	put_s("boot: trap: mstatus 0x");
	put_x(csr_read(mstatus));
	put_s(" mepc  0x");
	put_x(csr_read(mepc));
	put_s("\r\n"
	     "            mcause  0x");
	put_x(csr_read(mcause));
	put_s(" mtval 0x");
	put_x(csr_read(mbadaddr));
	put_s("\r\n");
#endif
	hang();
}

#ifdef HBWIF
static inline void hbwif_init(void)
{
	unsigned int i;
	scr->switcher_sel = SWITCHER_SEL_HBWIF;
	scr->hbwif_resets = 0;

	for (i = 0; i < HBWIF_NLANES; i++) {
		hbwif_t *hbwif = HBWIF_CONFIG_ADDR(i);
		hbwif->bert_enable = 0;
		hbwif->mem_mode_enable = 1;
	}
}
#endif /* HBWIF */

int main(void)
{
	int rc;

	uart->div = uart_divisor(115200U);
	uart->txctrl = UART_TXEN;
#ifdef HBWIF
	hbwif_init();
#else
	scr->switcher_sel = SWITCHER_SEL_LBWIF;
#endif

	if ((rc = sd_init()) ||
	    (rc = gpt_load(&gpt_type_eagle, PAYLOAD_BASE))) {
#ifdef DEBUG
		put_s("boot: error 0x");
		put_x(rc);
		put_s("\r\n");
#endif
		hang();
	}
	return rc;
}
