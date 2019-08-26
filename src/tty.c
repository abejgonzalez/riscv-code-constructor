#include <stdint.h>
#include <beagle/devices/uart.h>

#include "tty.h"
#include "common.h"

static uart_t * const uart = (uart_t *)(UART_ADDR);

static inline void uart_putc(char c)
{
#if __riscv_atomic
	int32_t r;
	for (;;) {
		__asm__ __volatile__ (
			"amoor.w %0, %2, %1\n"
			: "=r" (r), "+A" (uart->txfifo)
			: "r" (c)
		);
		if (r >= 0) {
			break;
		}
		relax();
	}
#else
	while ((int32_t)(uart->txfifo) < 0) {
		relax();
	}
	uart->txfifo = c;
#endif
}

void uart_puts(const char *s)
{
	while (*s != '\0') {
		uart_putc(*s++);
	}
}

void uart_putx(unsigned long x)
{
	unsigned long i = sizeof(x) << 3;
	do {
		unsigned long d;
		i -= 4;
		d = (x >> i) & 0xF;
		uart_putc(d < 0xA ? '0' + d : 'a' + d - 0xA);
	} while (i > 0);
}
