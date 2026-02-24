#include "IO.h"
#include "Windows.h"
#include "CPU.h"
#include <stdio.h>

uint8_t input_file_data[0x03F8];
uint8_t output_file_data[0x03F8];

int notes[108] =
{
	16,
	17,
	18,
	19,
	20,
	22,
	23,
	25,
	26,
	28,
	29,
	31,
	33,
	35,
	37,
	39,
	41,
	44,
	46,
	49,
	52,
	55,
	58,
	62,
	65,
	69,
	73,
	78,
	82,
	87,
	93,
	98,
	104,
	110,
	117,
	123,
	131,
	139,
	147,
	156,
	165,
	175,
	185,
	196,
	208,
	220,
	233,
	247,
	262,
	277,
	294,
	311,
	330,
	349,
	370,
	392,
	415,
	440,
	466,
	494,
	523,
	554,
	587,
	622,
	659,
	698,
	740,
	784,
	830,
	880,
	932,
	988,
	1047,
	1109,
	1175,
	1245,
	1319,
	1397,
	1480,
	1568,
	1661,
	1760,
	1865,
	1976,
	2093,
	2217,
	2349,
	2489,
	2637,
	2794,
	2960,
	3136,
	3322,
	3520,
	3729,
	3951,
	4186,
	4435,
	4699,
	4978,
	5274,
	5588,
	5920,
	6272,
	6645,
	7040,
	7459,
	7902
};

void IOFilesInit()
{
	FILE* input_file = NULL;
	FILE* output_file = NULL;
	errno_t err;

	//initialize input file
	for (int i = 0; i < sizeof(input_file_data); i++)
	{
		input_file_data[i] = 0x00;
	}
	err = fopen_s(&input_file, "./IO/lbc_input.txt", "wb");
	if (!err)
	{
		fwrite(input_file_data, sizeof(uint8_t), 0x03F8, input_file);
		fclose(input_file);
	}
	else
	{
		printf("IO input file error: %d", err);
	}

	//initialize output file
	for (int i = 0; i < sizeof(output_file_data); i++)
	{
		output_file_data[i] = 0x00;
	}
	err = fopen_s(&output_file, "./IO/lbc_output.txt", "wb");
	if (!err)
	{
		fwrite(output_file_data, sizeof(uint8_t), 0x03F8, output_file);
		fclose(output_file);
	}
	else
	{
		printf("IO output file error: %d", err);
	}

	for (int i = 0; i < sizeof(IO_address); i++)
	{
		IO_address[i] = 0x00;
	}
}

void UpdateIO()
{
	FILE* input_file = NULL;
	FILE* output_file = NULL;
	errno_t err;

	//read input file
	err = fopen_s(&input_file, "./IO/lbc_input.txt", "rb");
	if (!err)
	{
		fread(input_file_data, sizeof(uint8_t), 0x03F8, input_file);
		fclose(input_file);
	}
	else
	{
		printf("IO input file error: %d", err);
	}
	for (int i = 0; i < sizeof(input_file_data); i++)
	{
		IO_address[i + 0x000d] = input_file_data[i];
	}

	//initialize output file
	for (int i = 0; i < sizeof(output_file_data); i++)
	{
		output_file_data[i] = IO_address[i+sizeof(input_file_data)+0x000d];
	}
	err = fopen_s(&output_file, "./IO/lbc_output.txt", "wb");
	if (!err)
	{
		fwrite(output_file_data, sizeof(uint8_t), 0x03F8, output_file);
		fclose(output_file);
	}
	else
	{
		printf("IO output file error: %d", err);
	}
}

int PlaySpeaker(void* data)
{
	int last_cpu_stage = cpu_stage;
	while (1)
	{
		last_cpu_stage = cpu_stage;

		if (getValue(0x080d) > 0 && getValue(0x080d) < 108 && getValue(0x080e) > 0 && cpu_stage != last_cpu_stage)
		{
			Beep(notes[getValue(0x080d)], (getValue(0x080e) * 1000) / 0xFF);
		}
	}
}