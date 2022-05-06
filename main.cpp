#include <stdio.h>
#include "pico/stdlib.h"
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
        stdio_init_all();

        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);

        gCapSens.init(pio0, 0, 2, 0x0F);
        gCapSens.setIrqHandler(touchSensIrqHandler);
        gCapSens.setOverSampling(256);
        gCapSens.enable();

        while (1) {
            gpio_put(LED_PIN, 0);
            sleep_ms(250);
            gpio_put(LED_PIN, 1);
            puts("Hello World\n");
            sleep_ms(1000);
        }

    }
}