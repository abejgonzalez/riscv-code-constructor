#ifndef _BEAGLE_TTY_H
#define _BEAGLE_TTY_H

extern void uart_puts(const char *);
extern void uart_putx(unsigned long);

#ifdef DEBUG
#define put_s(s)         uart_puts((s))
#define put_x(x)         uart_putx((x))
#else
#define put_s(s)         do { } while (0)
#define put_x(x)         do { } while (0)
#endif

#endif /* _BEAGLE_TTY_H */
