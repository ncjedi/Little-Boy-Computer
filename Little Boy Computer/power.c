#include "CPU.h"
#include "GPU.h"
#include "Cart.h"
#include "bus.h"
#include "IO.h"
#include <threads.h>
#include <stdio.h>

int main()
{
	thrd_t sound_thread;
	thrd_t gpu_thread;
	thrd_t IO_thread;
	prog_rom[0] = 0x6d; //set the cpu to initially jump back to it's own position infinitely
	prog_rom[1] = 0x10;
	prog_rom[2] = 0x01;

	//GPUinit();
	IOFilesInit();
	thrd_create(&sound_thread, PlaySpeaker, NULL);
	thrd_create(&gpu_thread, GPUinit, NULL);
	thrd_create(&IO_thread, UpdateIO, NULL);
	CPU_clock();
	thrd_join(sound_thread, NULL);
}