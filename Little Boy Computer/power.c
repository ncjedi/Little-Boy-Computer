#include "CPU.h"
#include "GPU.h"
#include "Cart.h"
#include "bus.h"
#include "IO.h"
//#include <threads.h>

int main()
{
	//thrd_t sound_thread;
	prog_rom[0] = 0x6d; //set the cpu to initially jump back to it's own position infinitely
	prog_rom[1] = 0x10;
	prog_rom[2] = 0x01;

	setValue(0x080c, 70);
	setValue(0x080d, 0xff);

	GPUinit();
	//thrd_create(&sound_thread, PlaySpeaker, NULL);
	CPU_clock();
	//thrd_join(sound_thread, NULL);
}