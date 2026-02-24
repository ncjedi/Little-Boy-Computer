#pragma once
#include "bus.h"
#include <stdint.h>

uint8_t IO_address[0x07FF]; //IO addresses (memory addresses 0x0802 - 0x1000)

void IOFilesInit();
int UpdateIO(void* data);
int PlaySpeaker(void* data);