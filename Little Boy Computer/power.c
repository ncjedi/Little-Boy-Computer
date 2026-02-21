#include "CPU.h"
#include "GPU.h"
#include "Cart.h"

int main()
{
	prog_rom[0] = 0x6d; //set the cpu to initially jump back to it's own position infinitely
	prog_rom[1] = 0x10;
	prog_rom[2] = 0x01;

	GPUinit();
	CPU_clock();
}