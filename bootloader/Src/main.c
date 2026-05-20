#include "stm32f4xx.h"
#include "crc32.h"

#define appAddress    0x08010000
#define appSize       0x00010000
#define crcFooterAddr (appAddress + appSize - 4)

typedef void (*funcPtr)(void);

void errorBlink(void) {
    while (1) {
        GPIOA->ODR |= (1 << 5);
        for (volatile int d = 0; d < 1000000; d++);
        GPIOA->ODR &= ~(1 << 5);
        for (volatile int d = 0; d < 1000000; d++);
    }
}

void jumpToApp(void) {
    uint32_t appStackPtr     = *((volatile uint32_t*) appAddress);
    uint32_t appResetHandler = *((volatile uint32_t*) (appAddress + 4));
    SCB->VTOR = appAddress;
    __set_MSP(appStackPtr);
    funcPtr jump = (funcPtr) appResetHandler;
    jump();
}

int main(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= (1 << 10);

    /* blink 3 times - bootloader is running */
    for (int i = 0; i < 3; i++) {
        GPIOA->ODR |= (1 << 5);
        for (volatile int d = 0; d < 500000; d++);
        GPIOA->ODR &= ~(1 << 5);
        for (volatile int d = 0; d < 500000; d++);
    }

    /* read expected CRC from footer */
    uint32_t expectedCrc = *((volatile uint32_t*) crcFooterAddr);

    /* calculate CRC over app binary excluding footer */
    uint32_t calculatedCrc = crc32Calculate(
        (const uint8_t*) appAddress,
        appSize - 4
    );

    /* if mismatch - app is corrupted */
    if (calculatedCrc != expectedCrc) {
        errorBlink();
    }

    jumpToApp();
    while (1);
}