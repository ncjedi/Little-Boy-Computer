#pragma once
#include "bus.h"
#include <stdint.h>

uint8_t IO_address[0x0200]; //IO addresses (memory addresses 0x0801 - 0x1000)

int PlaySpeaker(void* data);