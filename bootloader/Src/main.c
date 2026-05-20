#include "stm32f4xx.h"


/* The app's vector table lives at this address in flash */
#define appAddress 0x08010000


/* This is a function pointer type - it points to a function that takes no arguments and returns nothing */
typedef void (*funcPtr)(void);

void jumpToApp(void) {

    /* Step 1: read the app's stack pointer */
    uint32_t appStackPtr = *((volatile uint32_t*) appAddress);

    /* Step 2: read the app's Reset_Handler address */
    uint32_t appResetHandler = *((volatile uint32_t*) (appAddress + 4));

    /* Step 3: relocate the vector table to the app's address */
    SCB->VTOR = appAddress;

    /* Step 4: set the stack pointer */
    __set_MSP(appStackPtr);

    /* Step 5: create function pointer and jump */
    funcPtr jump = (funcPtr) appResetHandler;
    jump();
}

int main(void){
    /*Enable GPIOA clock*/
    RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;

    /*Set PA5 as output*/
    GPIOA->MODER |= (1<<10);
    /* Blink 3 times to show bootloader is running */
    for (int i = 0; i < 3; i++) {
        GPIOA->ODR |= (1 << 5);   /* LED on */
        for (volatile int d = 0; d < 500000; d++);
        GPIOA->ODR &= ~(1 << 5);  /* LED off */
        for (volatile int d = 0; d < 500000; d++);
    }
    /*Jump to the app*/
    jumpToApp();

    /*should never reach here*/
    while(1);
}