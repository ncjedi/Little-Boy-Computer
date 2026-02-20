#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <conio.h>
#include "CPU.h"
#include "bus.h"
#include "GPU.h"

uint8_t reg_x = 0x00;
uint8_t reg_y = 0x00;
uint8_t reg_z = 0x00;
uint8_t accumulator = 0x00;
uint8_t flags = 0x00;
uint16_t address = 0x0000; //address taken from input in memory
uint16_t program_counter = 0x1000; //location the CPU is currently looking at in memory.
int cpu_stage = 0;
int key_pressed;
int fa_key; //the second value in the buffer for function and arrow keys

void CPU_clock()
{
	clock_t start_time = clock();
	clock_t end_time = clock();

	while (1)
	{
		float elapsed_time = ((float)end_time - (float)start_time) / CLOCKS_PER_SEC;
		if (elapsed_time >= 0.000001)
		{
			start_time = clock();
			GPUtick();
			if (cpu_stage)
			{
				cpu_stage = 0;
			}
			else
			{
				cpu_stage = 1;
			}
		}

		/*//check for key presses
		if (_kbhit())
		{
			key_pressed = _getch();

			//if a function key is pressed
			if (key_pressed == 0)
			{
				fa_key = _getch();

				if (fa_key == 62) //F4
				{
					printf("IN");
					readCart();
				}
			}
		}*/

		end_time = clock();
	}
}

void ADC_I()
{
	accumulator = accumulator + getValue(program_counter);
}