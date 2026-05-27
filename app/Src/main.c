#include "stm32f4xx.h"
#include "spi.h"
#include "ili9341.h"

/* simple delay */
static void delay(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 16000; i++);
}

int main(void) {

    /* enable GPIOA clock and set PA5 as output for debug LED */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= (1 << 10);

    /* blink once to show we reached main */
    GPIOA->ODR |= (1 << 5);
    delay(500);
    GPIOA->ODR &= ~(1 << 5);
    delay(500);

    /* init SPI */
    spiInit();

    /* blink twice to show SPI init done */
    for (int i = 0; i < 2; i++) {
        GPIOA->ODR |= (1 << 5);
        delay(300);
        GPIOA->ODR &= ~(1 << 5);
        delay(300);
    }

    /* init display */
    ili9341Init();

    /* blink three times to show display init done */
    for (int i = 0; i < 3; i++) {
        GPIOA->ODR |= (1 << 5);
        delay(300);
        GPIOA->ODR &= ~(1 << 5);
        delay(300);
    }

    /* fill screen blue */
    ili9341FillScreen(COLOR_BLUE);

    /* blink rapidly forever to show fill done */
    while (1) {
        GPIOA->ODR |= (1 << 5);
        delay(100);
        GPIOA->ODR &= ~(1 << 5);
        delay(100);
    }
}