#ifndef UART_H
#define UART_H

#include "stm32f4xx.h"

/* initialize UART2 on PA2 (TX) and PA3 (RX) at 115200 baud */
void uartInit(void);

/* send a single character */
void uartSendChar(char c);

/* send a string */
void uartSendString(const char *str);

#endif