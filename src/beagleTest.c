#include <beagle/devices/scr.h>
#include <beagle/devices/uart.h>
#include <beagle/devices/gpio.h>
#include <beagle/sections.h>
#include <beagle/csr.h>

#include "sd.h"
#include "gpt.h"
#include "tty.h"
#include "common.h"
#include "boot.h"

#include <stdio.h>

static scr_t * const scr = (scr_t *)(SCR_ADDR);
static gpio_t * const gpio = (gpio_t *)(GPIO_ADDR);
static uart_t * const uart = (uart_t *)(UART_ADDR);

#define GPIO_TEST_VAL 0x7
#define GPIO_TEST_OUT_VAL 0x38

int main(void)
{
	int rc;

    //printf("Start: SCR Test\n");
    //scr->uncore_clk_pass_sel = 0;
    //printf("Wrote to SCR\n");
    //scr->uncore_clk_pass_sel = 1;
    //printf("End: SCR Test\n");

    //printf("Start: GPIO Test\n");
    //gpio->output_en = 0x7;
    //gpio->input_en = 0x38;

    //gpio->output_value = GPIO_TEST_VAL;
    //uint32_t in_val = gpio->input_value;
    //printf("READ GPIO = 0x%x\n", in_val);

    //if (in_val != GPIO_TEST_OUT_VAL) {
    //    printf("ERROR\n");
    //}

    //printf("End: GPIO Test\n");

    printf("Start: UART setup\n");
	uart->div = uart_divisor(115200U);
	uart->txctrl = UART_TXEN;

    printf("Start: SDCard setup\n");
	rc = sd_init();
	printf("sd_init: retcode: %d\n", rc);
	rc = gpt_load(&gpt_type_eagle, (void*)(0x50000000));
	printf("gpt_load: retcode: %d\n", rc);
//	if ((rc = sd_init()) ||
//	    (rc = gpt_load(&gpt_type_eagle, (void*)(0x50000000)))) {
//#ifdef DEBUG
//		put_s("boot: error 0x");
//		put_x(rc);
//		put_s("\r\n");
//#endif
//        printf("Progam hung\n");
//		hang();
//	}
	return rc;
}
