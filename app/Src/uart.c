#include "uart.h"

void uartInit(void) {
    /* enable clocks for UART2 and GPIOA */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* configure PA2 (TX) and PA3 (RX) as alternate function */
    GPIOA->MODER |= (2 << 4) | (2 << 6);

    /* set alternate function to AF7 (UART2) for PA2 and PA3 */
    GPIOA->AFR[0] |= (7 << 8) | (7 << 12);

    /* configure UART2:
       baud rate = 115200
       system clock = 16MHz (default HSI)
       BRR = 16000000 / 115200 = 138.88 = 0x8B */
    USART2->BRR = 0x8B;

    /* enable UART, TX and RX */
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void uartSendChar(char c) {
    /* wait until transmit buffer is empty */
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void uartSendString(const char *str) {
    while (*str) {
        uartSendChar(*str++);
    }
}