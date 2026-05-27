#include "spi.h"

void spiInit(void) {
    /* enable clocks for SPI1, GPIOA and GPIOB */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    /* configure PA5 (SCK), PA6 (MISO), PA7 (MOSI) as alternate function */
    GPIOA->MODER |= (2 << 10) | (2 << 12) | (2 << 14);

    /* set alternate function to AF5 (SPI1) for PA5, PA6, PA7 */
    GPIOA->AFR[0] |= (5 << 20) | (5 << 24) | (5 << 28);

    /* configure PB6 (CS) as output */
    GPIOB->MODER |= (1 << 12);

    /* set CS high - not talking to anyone yet */
    GPIOB->ODR |= (1 << 6);

    /* configure SPI1:
       MSTR = 1    - STM32 is the master
       BR   = 011  - clock divider /16, slower and more reliable for init
       SSM  = 1    - software slave management
       SSI  = 1    - internal slave select high
       SPE  = 1    - enable SPI */
    SPI1->CR1 = SPI_CR1_MSTR |
                SPI_CR1_BR_1 |
                SPI_CR1_BR_0 |
                SPI_CR1_SSM  |
                SPI_CR1_SSI  |
                SPI_CR1_SPE;
}

void spiSendByte(uint8_t byte) {
    /* wait until transmit buffer is empty */
    while (!(SPI1->SR & SPI_SR_TXE));

    /* send the byte */
    *((volatile uint8_t*)&SPI1->DR) = byte;

    /* wait until transmission is complete */
    while (SPI1->SR & SPI_SR_BSY);
}

void spiSendBuffer(uint8_t *buffer, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        spiSendByte(buffer[i]);
    }
}