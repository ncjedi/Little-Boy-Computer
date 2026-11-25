#pragma once

#include <stdint.h>

uint8_t getValue(uint16_t address);
void setValue(uint16_t address, uint8_t value);
void setScanline(uint8_t value);
uint8_t getGPUInterrupt();