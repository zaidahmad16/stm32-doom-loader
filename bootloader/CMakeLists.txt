cmake_minimum_required(VERSION 3.20)
project(stm32-doom-loader C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)

set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS "${MCU_FLAGS} -Wall -O0 -g -DSTM32F401xE")
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -T${CMAKE_SOURCE_DIR}/STM32F401RETX_FLASH.ld -nostdlib -Wl,-Map=output.map")

include_directories(Inc)

file(GLOB SOURCES 
    "${CMAKE_SOURCE_DIR}/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Startup/*.s"
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})