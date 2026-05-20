# stm32-doom-loader

A bare-metal bootloader for the STM32F401RE. It verifies a staged app with CRC32
and jumps to it. The end goal is running Doom on an ILI9341 SPI display with the
WAD file loaded from SD card.

No HAL. No RTOS. Just the chip, a linker script, and a startup file written from scratch.

---

## Structure

bootloader/   lives at 0x08000000, handles CRC check and jump
app/          lives at 0x08010000, currently a test blinky
tools/        append_crc.py — pads app binary to 64K and appends CRC footer

---

## Memory Map

0x08000000   bootloader (32K)
0x08008000   reserved gap
0x08010000   app (64K)
0x0801FFFC   CRC32 footer
0x08020000   unused flash
0x20000000   RAM — .data, .bss, heap, stack
0x20018000   top of RAM

---

## How it works

On power on the chip reads from 0x08000000. The bootloader runs, blinks the LED
three times, then calculates CRC32 over the 64K app region at 0x08010000. It reads
the expected CRC from the last 4 bytes of that region and compares. Match means jump.
No match means rapid blink forever — the app is corrupted or missing.

The Python script pads the compiled app binary to 64K and appends the CRC footer
before flashing. Both sides have to agree on the same 64K region or the check fails.

---

## Hardware

- STM32 Nucleo F401RE
- ILI9341 2.4" SPI TFT LCD with onboard SD card slot (Phase 4)
- Tactile buttons x6 for Doom controls (Phase 4)

---

## Progress

- [x] Phase 1 — bare-metal blinky, toolchain from scratch
- [x] Phase 2 — linker script split, bootloader jump to app
- [x] Phase 3 — CRC32 verification, corruption detection
- [ ] Phase 4 — ILI9341 driver, SD card, Doom