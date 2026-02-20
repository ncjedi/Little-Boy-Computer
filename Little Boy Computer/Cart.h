#pragma once

#include <stdint.h>

uint8_t prog_rom[0x6FFF]; //program memory
uint8_t graphic_rom[0x7FFF]; //graphic memory

void readCart();