#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "capTouchSens.h"

const uint LED_PIN = 25;

capTouchSens<8> gCapSens;

void touchSensIrqHandler()
{
    gCapSens.irqHandler();
}

extern "C"
{
    int main() {

        clocks_init ();
        // sdk uses XOSC_MHZ as base clock for pll setup 
/*        pll_init(pll_sys, 1, 1500 * MHZ, 6, 2);
        pll_init(pll_usb, 1, 480 * MHZ, 5, 2);

        clock_configure(    clk_sys, 
                            CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, 
                            CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 
                            125 * MHZ, 
                            125 * MHZ);

        clock_configure(    clk_usb, 
                            0, 
                            CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, 
                            48 * MHZ, 
                            48 * MHZ);

        clock_configure(    clk_peri, 
                            0, 
                            CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 
                            125 * MHZ, 
                            125 * MHZ);                   
*/
        stdio_init_all();

        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);

        gCapSens.init(pio0, 0, 2, 0x0F, 0xFFFF, 0x003F);
        gCapSens.setIrqHandler(touchSensIrqHandler);
        gCapSens.setOverSampling(256);
        gCapSens.enable();
        
        while (1) {
            gpio_put(LED_PIN, 0);
            sleep_ms(250);
            gpio_put(LED_PIN, 1);
            sleep_ms(250);
            for( int i = 0; i < 8; i++)
            {
                printf("%4d ", gCapSens.capVal[i]);
            }
            puts("\r");
        }

    }
}