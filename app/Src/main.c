#include "stm32f4xx.h"

int main(void) {

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Set PA5 as output */
    GPIOA->MODER |= (1 << 10);

    /* Blink slowly forever - this proves the bootloader jumped here */
    while (1) {
        GPIOA->ODR |= (1 << 5);   /* LED on */
        for (volatile int d = 0; d < 2000000; d++);  /* long delay */
        GPIOA->ODR &= ~(1 << 5);  /* LED off */
        for (volatile int d = 0; d < 2000000; d++);  /* long delay */
    }
}