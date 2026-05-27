#ifndef ILI9341_H
#define ILI9341_H

#include "stm32f4xx.h"

/* display dimensions */
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320

/* common colors in RGB565 format */
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F

/* initialize the ILI9341 display */
void ili9341Init(void);

/* fill the entire screen with one color */
void ili9341FillScreen(uint16_t color);

/* draw a single pixel at x, y with a color */
void ili9341DrawPixel(uint16_t x, uint16_t y, uint16_t color);

#endif