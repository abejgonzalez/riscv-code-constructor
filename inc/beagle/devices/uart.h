#ifndef _BEAGLE_UART_H
#define _BEAGLE_UART_H

#include <beagle/const.h>

#ifndef __ASSEMBLY__
#include <stdint.h>

typedef volatile struct uart {
	uint32_t txfifo;
	uint32_t rxfifo;
	uint32_t txctrl;
	uint32_t rxctrl;
	uint32_t ie;
	uint32_t ip;
	uint32_t div;
} __attribute__ ((packed)) uart_t;

static inline unsigned long uart_divisor(unsigned long baud)
{
	unsigned long n;
	n = (REFCLK + baud - 1) / baud;
	return (n > 0) ? (n - 1) : 0;
}

#endif /* !__ASSEMBLY__ */

#define UART_ADDR               _AC(0xC000,UL)

/* TXCTRL register */
#define UART_TXEN               0x1
#define UART_TXNSTOP            0x2
#define UART_TXWM(x)            (((x) & 0xffff) << 16)

/* RXCTRL register */
#define UART_RXEN               0x1
#define UART_RXWM(x)            (((x) & 0xffff) << 16)

/* IP register */
#define UART_IP_TXWM            0x1
#define UART_IP_RXWM            0x2

#endif /* _BEAGLE_UART_H */
