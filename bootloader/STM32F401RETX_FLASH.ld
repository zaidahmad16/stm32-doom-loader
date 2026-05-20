ENTRY(Reset_Handler)

MEMORY
{
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 96K
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 32K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } >FLASH

    .data :
    {
        . = ALIGN(4);
        *(.data)
        *(.data*)
        . = ALIGN(4);
    } >RAM AT> FLASH

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM
}