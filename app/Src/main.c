#include "stm32f4xx.h"
#include "spi.h"
#include "ili9341.h"
#include "uart.h"

static void delay(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 16000; i++);
}

int main(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= (1 << 10);

    uartInit();
    uartSendString("booted\r\n");

    spiInit();
    uartSendString("spi init done\r\n");

    ili9341Init();
    uartSendString("display init done\r\n");

    ili9341FillScreen(COLOR_RED);
    uartSendString("fill done\r\n");

    while (1) {
        GPIOA->ODR ^= (1 << 5);
        delay(100);
    }
}