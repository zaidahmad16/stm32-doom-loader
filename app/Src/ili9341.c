#include "ili9341.h"
#include "spi.h"

/* CS pin - PB6 */
#define CS_LOW()    GPIOB->ODR &= ~(1 << 6)
#define CS_HIGH()   GPIOB->ODR |=  (1 << 6)

/* DC pin - PC7 (data/command select)
   LOW  = command
   HIGH = data */
#define DC_LOW()    GPIOC->ODR &= ~(1 << 7)
#define DC_HIGH()   GPIOC->ODR |=  (1 << 7)

/* RESET pin - PA9
   LOW  = reset active
   HIGH = reset inactive */
#define RESET_LOW()  GPIOA->ODR &= ~(1 << 9)
#define RESET_HIGH() GPIOA->ODR |=  (1 << 9)

/* simple delay loop */
static void delay(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 16000; i++);
}

/* send a command byte to the display */
static void sendCommand(uint8_t cmd) {
    DC_LOW();
    CS_LOW();
    spiSendByte(cmd);
    CS_HIGH();
}

/* send a data byte to the display */
static void sendData(uint8_t data) {
    DC_HIGH();
    CS_LOW();
    spiSendByte(data);
    CS_HIGH();
}

void ili9341Init(void) {
    /* enable GPIOC clock for DC pin */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* configure PC7 as output (DC pin) */
    GPIOC->MODER |= (1 << 14);

    /* configure PA9 as output (RESET pin) */
    GPIOA->MODER |= (1 << 18);

    /* hardware reset */
    RESET_HIGH();
    delay(10);
    RESET_LOW();
    delay(20);
    RESET_HIGH();
    delay(150);

    /* initialize display registers */
    sendCommand(0x01);  /* software reset */
    delay(150);

    sendCommand(0x11);  /* sleep out */
    delay(150);

    sendCommand(0x3A);  /* pixel format */
    sendData(0x55);     /* 16 bit RGB565 */

    sendCommand(0x36);  /* memory access control */
    sendData(0x48);     /* portrait mode */

    sendCommand(0x29);  /* display on */
    delay(10);
}

/* set the drawing window */
static void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    sendCommand(0x2A);
    sendData(x0 >> 8);
    sendData(x0 & 0xFF);
    sendData(x1 >> 8);
    sendData(x1 & 0xFF);

    sendCommand(0x2B);
    sendData(y0 >> 8);
    sendData(y0 & 0xFF);
    sendData(y1 >> 8);
    sendData(y1 & 0xFF);

    sendCommand(0x2C);
}

void ili9341FillScreen(uint16_t color) {
    setWindow(0, 0, ILI9341_WIDTH - 1, ILI9341_HEIGHT - 1);

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    DC_HIGH();
    CS_LOW();

    for (uint32_t i = 0; i < ILI9341_WIDTH * ILI9341_HEIGHT; i++) {
        spiSendByte(hi);
        spiSendByte(lo);
    }

    CS_HIGH();
}

void ili9341DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    setWindow(x, y, x, y);

    DC_HIGH();
    CS_LOW();
    spiSendByte(color >> 8);
    spiSendByte(color & 0xFF);
    CS_HIGH();
}