#include "bus.h"
#include "RAM.h"
#include "GPU.h"
#include "VRAM.h"
#include "Cart.h"

uint16_t RAMoffset = 0x0000;
uint16_t back_ramOffset = 0x0401;
uint16_t front_ramOffset = 0x0601;

uint16_t prog_romOffset = 0x1001;
uint16_t graphic_romOffset = 0x8000;

uint8_t getValue(uint16_t address)
{
	if (address <= 0x0400)
	{
		return RAM[address - RAMoffset]; /*RAM addresses*/
	}
	else if (address <= 0x0600)
	{
		return back_ram[address - back_ramOffset]; /*VRAM back addresses also if you want to read from it for some reason!*/
	}
	else if (address <= 0x0800)
	{
		return front_ram[address - front_ramOffset]; /*VRAM foreground addresses... if you wanted to read from those for some reason?*/
	}
	else if (address <= 0x1000)
	{
		/*read from I/O*/
	}
	else if (address <= 0x7FFF)
	{
		return prog_rom[address - prog_romOffset]; /*read from program rom*/
	}
	else
	{
		return graphic_rom[address - graphic_romOffset]; /*read from graphic rom*/
	}
	return 0x0000;
}

void setValue(uint16_t address, uint8_t value)
{
	if (address <= 0x0400)
	{
		RAM[address - RAMoffset] = value; /*write to RAM*/
	}
	else if (address <= 0x0600)
	{
		back_ram[address - back_ramOffset] = value; /*VRAM back addresses*/
	}
	else if (address <= 0x0800)
	{
		front_ram[address - front_ramOffset] = value; /*VRAM foreground addresses*/
	}
	else if (address <= 0x1000)
	{
		/*write to I/O*/
	}

	//these would be for writing to rom but it's rom so no.
	else if (address <= 0x7FFF)
	{
		/*read from program rom*/
	}
	else
	{
		/*read from graphic rom*/
	}
}