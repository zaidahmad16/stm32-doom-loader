#include "ili9341.h"
#include "spi.h"

#define CS_LOW()     GPIOB->ODR &= ~(1 << 6)
#define CS_HIGH()    GPIOB->ODR |=  (1 << 6)
#define DC_LOW()     GPIOC->ODR &= ~(1 << 7)
#define DC_HIGH()    GPIOC->ODR |=  (1 << 7)
#define RESET_LOW()  GPIOA->ODR &= ~(1 << 8)
#define RESET_HIGH() GPIOA->ODR |=  (1 << 8)

static void delay(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 16000; i++);
}

static void sendCommand(uint8_t cmd) {
    DC_HIGH();
    CS_LOW();
    spiSendByte(cmd);
    CS_HIGH();
}

static void sendData(uint8_t data) {
    DC_LOW();
    CS_LOW();
    spiSendByte(data);
    CS_HIGH();
}

void ili9341Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= (1 << 14);  /* PC7 as output - DC */
    GPIOA->MODER |= (1 << 16);  /* PA8 as output - RESET */

    RESET_HIGH();
    delay(50);
    RESET_LOW();
    delay(100);
    RESET_HIGH();
    delay(200);

    sendCommand(0x01);
    delay(200);

    sendCommand(0xCB);
    sendData(0x39);
    sendData(0x2C);
    sendData(0x00);
    sendData(0x34);
    sendData(0x02);

    sendCommand(0xCF);
    sendData(0x00);
    sendData(0xC1);
    sendData(0x30);

    sendCommand(0xE8);
    sendData(0x85);
    sendData(0x00);
    sendData(0x78);

    sendCommand(0xEA);
    sendData(0x00);
    sendData(0x00);

    sendCommand(0xED);
    sendData(0x64);
    sendData(0x03);
    sendData(0x12);
    sendData(0x81);

    sendCommand(0xF7);
    sendData(0x20);

    sendCommand(0xC0);
    sendData(0x23);

    sendCommand(0xC1);
    sendData(0x10);

    sendCommand(0xC5);
    sendData(0x3E);
    sendData(0x28);

    sendCommand(0xC7);
    sendData(0x86);

    sendCommand(0x36);
    sendData(0x48);

    sendCommand(0x3A);
    sendData(0x55);

    sendCommand(0xB1);
    sendData(0x00);
    sendData(0x18);

    sendCommand(0xB6);
    sendData(0x08);
    sendData(0x82);
    sendData(0x27);

    sendCommand(0xF2);
    sendData(0x00);

    sendCommand(0x26);
    sendData(0x01);

    sendCommand(0xE0);
    sendData(0x0F);
    sendData(0x31);
    sendData(0x2B);
    sendData(0x0C);
    sendData(0x0E);
    sendData(0x08);
    sendData(0x4E);
    sendData(0xF1);
    sendData(0x37);
    sendData(0x07);
    sendData(0x10);
    sendData(0x03);
    sendData(0x0E);
    sendData(0x09);
    sendData(0x00);

    sendCommand(0xE1);
    sendData(0x00);
    sendData(0x0E);
    sendData(0x14);
    sendData(0x03);
    sendData(0x11);
    sendData(0x07);
    sendData(0x31);
    sendData(0xC1);
    sendData(0x48);
    sendData(0x08);
    sendData(0x0F);
    sendData(0x0C);
    sendData(0x31);
    sendData(0x36);
    sendData(0x0F);

    sendCommand(0x11);
    delay(150);

    sendCommand(0x29);
    delay(100);
}

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

    DC_LOW();
    CS_LOW();

    for (uint32_t i = 0; i < ILI9341_WIDTH * ILI9341_HEIGHT; i++) {
        spiSendByte(hi);
        spiSendByte(lo);
    }

    CS_HIGH();
}

void ili9341DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    setWindow(x, y, x, y);

    DC_LOW();
    CS_LOW();
    spiSendByte(color >> 8);
    spiSendByte(color & 0xFF);
    CS_HIGH();
}