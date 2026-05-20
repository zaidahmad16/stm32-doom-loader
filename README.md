# stm32-doom-loader

Bare-metal bootloader for the STM32F401RE that verifies and jumps to a staged app at 0x08010000. End goal: boot Doom on an ILI9341 display with WAD loaded from SD card.

- `bootloader/` — lives at 0x08000000
- `app/` — lives at 0x08010000