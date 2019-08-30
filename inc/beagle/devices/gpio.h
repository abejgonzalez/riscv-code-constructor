#ifndef _BEAGLE_GPIO_H
#define _BEAGLE_GPIO_H

#include <beagle/const.h>

#ifndef __ASSEMBLY__
#include <stdint.h>

typedef volatile struct gpio {
	uint32_t input_value;
	uint32_t input_en;
	uint32_t output_en;
	uint32_t output_value;
	uint32_t pue;
	uint32_t ds;
	uint32_t rise_ie;
	uint32_t rise_ip;
	uint32_t fall_ie;
	uint32_t fall_ip;
	uint32_t high_ie;
	uint32_t high_ip;
	uint32_t low_ie;
	uint32_t low_ip;
	uint32_t __reserved_0x38;
	uint32_t __reserved_0x3c;
	uint32_t out_xor;
} __attribute__ ((packed)) gpio_t;

#endif /* !__ASSEMBLY__ */

#define GPIO_ADDR       _AC(0x9000,UL)

#endif /* _BEAGLE_GPIO_H */
