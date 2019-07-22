// ----------------------------------------------------------------
// SiFive Block i2c
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#ifndef I2C_UTIL_H
#define I2C_UTIL_H

#define PRESCALE_LO_B 0x0
#define PRESCALE_HI_B 0x4
#define CTRL_BASE  0x8
#define CTRL_SET_RES 0
#define CTRL_SET_INT_ENB 6
#define CTRL_SET_CORE_ENB 7
#define TX_RX_DATA 0xC
#define CHANGE_CMD 0x10 // r -> report status, w -> update i2c command

volatile char* uart_h;

#endif
