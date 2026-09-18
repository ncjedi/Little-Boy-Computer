#pragma once

#include <stdint.h>

uint8_t back_ram[512]; //background ram (memory addresses 0x0401 - 0x0600)
uint8_t front_ram[512]; //foreground ram (memory addresses 0x0602 - 0x0800)
uint8_t GPU_interrupt; //memory address 0x0601