.syntax unified        /* use unified ARM/Thumb syntax */
.cpu cortex-m4         /* we are targeting the Cortex-M4 CPU */
.thumb                 /* use the Thumb instruction set */

/* This section is the vector table - it must be placed at the very
   start of flash so the chip can find it on power on */
.section .isr_vector, "a", %progbits
.word 0x20018000          /* stack pointer - top of RAM */
.word Reset_Handler       /* power on - first function to run */
.word HardFault_Handler   /* called if a memory or bus error occurs */
.word 0                   /* not used */
.word 0                   /* not used */

/* All our code goes in the .text section which lives in FLASH */
.section .text

.global Reset_Handler
.thumb_func               /* tell assembler this is a Thumb function */
Reset_Handler:

    /* Zero out .bss - these are variables that should start at 0
       RAM is not guaranteed to be zero on power on so we do it manually */
    ldr r0, =_sbss        /* r0 = start address of .bss */
    ldr r1, =_ebss        /* r1 = end address of .bss */
    mov r2, #0            /* r2 = 0, this is what we write */

zero_loop:
    cmp r0, r1            /* have we reached the end of .bss? */
    bge main_jump         /* if yes, jump to main */
    str r2, [r0]          /* write 0 to current address */
    add r0, r0, #4        /* move to next 4 byte address */
    b zero_loop           /* loop back */

main_jump:
    bl main               /* call main() - your actual program starts here */

/* If main() ever returns the chip would execute garbage memory
   so we just loop forever to stay safe */
loop_forever:
    b loop_forever

.global HardFault_Handler
.thumb_func
HardFault_Handler:
    b HardFault_Handler   /* loop forever if something goes wrong */