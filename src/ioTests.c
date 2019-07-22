#include <stdio.h>
#include "gpio.h"
#include "uart.h"

int main (void){
    printf("start gpio test\n");

    GPIO_SET_BASE(0x9000);
    GPIO_OUT_ENB(0x7);
    GPIO_IN_ENB(0x38);

    GPIO_OUT_WRITE(0x7);
    printf("READ: 0x%x\n", GPIO_IN_READ());

    printf("end gpio test\n");

    printf("start uart test\n");

    UART_SET_BASE(0xc000);
    UART_TX_CTRL(0x3);
    UART_RX_CTRL(0x1);

    UART_WRITE(0x18);
    UART_WRITE(0xFF);

    printf("Data read: 0x%x\n", UART_READ());
    printf("Data read: 0x%x\n", UART_READ());

    printf("end uart test\n");
}
