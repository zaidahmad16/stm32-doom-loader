#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"

/* initialize SPI1 for display communication */
void spiInit(void);

/* send a single byte over SPI1 */
void spiSendByte(uint8_t byte);

/* send multiple bytes over SPI1 */
void spiSendBuffer(uint8_t *buffer, uint32_t length);

#endif