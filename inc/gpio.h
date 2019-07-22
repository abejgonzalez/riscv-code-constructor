// ----------------------------------------------------------------
// SiFive Block GPIO
// ----------------------------------------------------------------
// Currently supports up to 32 GPIO ports
// ----------------------------------------------------------------

#ifndef GPIO_UTIL_H
#define GPIO_UTIL_H

// offsets for gpio
#define PIN_I_ENB    (0x04)
#define I_VAL        (0x00)
#define PIN_O_ENB    (0x08)
#define O_VAL        (0x0C)
#define R_INT_ENB    (0x18)
#define R_INT_PEND   (0x1C)
#define F_INT_ENB    (0x20)
#define F_INT_PEND   (0x24)
#define HI_INT_ENB   (0x28)
#define HI_INT_PEND  (0x2C)
#define LO_INT_ENB   (0x30)
#define LO_INT_PEND  (0x34)
#define INTNL_PU_ENB (0x10)
#define PIN_DRV_STR  (0x14)
#define O_XOR_ENB    (0x40)
#define PASS_AHI_INT_ENB (0x44)
#define PASS_ALO_INT_ENB (0x48)

// support up to 32 GPIOs
volatile uint8_t* gpio_h;

#define GPIO_SET_BASE(addr) ({ \
            gpio_h = (uint8_t*)(addr); \
        })

#define GPIO_OUT_ENB(val) ({ \
            *(gpio_h + PIN_O_ENB) = (val); \
        })

#define GPIO_IN_ENB(val) ({ \
            *(gpio_h + PIN_I_ENB) = (val); \
        })

#define GPIO_OUT_WRITE(val) ({ \
            *(gpio_h + O_VAL) = (val); \
        })

#define GPIO_IN_READ() ({ \
            *(gpio_h + I_VAL); \
        })

#endif
