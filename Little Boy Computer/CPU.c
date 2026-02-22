#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <conio.h>
#include "CPU.h"
#include "bus.h"
#include "GPU.h"
#include "IO.h"

uint8_t reg_x = 0x00;
uint8_t reg_y = 0x00;
uint8_t reg_z = 0x00;
uint8_t accumulator = 0x00;
uint8_t flags = 0x00;
uint16_t address = 0x0000; //address taken from input in memory
uint16_t program_counter = 0x1001; //location the CPU is currently looking at in memory.
uint8_t stack_counter = 0x00; //current location on the stack
int cpu_stage = 0;
int key_pressed;
int fa_key; //the second value in the buffer for function and arrow keys
int wait = 0; //how long to wait after certain actions to make the CPU not do multiple actions at once

void run_opcode()
{
	uint8_t opcode = getValue(program_counter);
	program_counter++;

	switch (opcode)
	{
	//NOP
	case 0x00: 
		NOP(); 
		break;

	//Access
	case 0x10: 
		LDA_IM(); 
		break; 
	case 0x11: 
		LDA_AB(); 
		break; 
	case 0x12: 
		LDA_ABX(); 
		break;
	case 0x13:
		LDA_ABY();
		break;
	case 0x14:
		LDA_INDX();
		break;
	case 0x15:
		LDA_INDY();
		break;
	case 0x16:
		STA_AB();
		break;
	case 0x17:
		STA_ABX();
		break;
	case 0x18:
		STA_ABY();
		break;
	case 0x19:
		STA_INDX();
		break;
	case 0x1a:
		STA_INDY();
		break;
	case 0x1b:
		LDX_IM();
		break;
	case 0x1c:
		LDX_AB();
		break;
	case 0x1d:
		LDX_ABY();
		break;
	case 0x1e:
		STX();
		break;
	case 0x1f:
		LDY_IM();
		break;
	case 0x20:
		LDY_AB();
		break;
	case 0x21:
		LDY_ABX();
		break;
	case 0x22:
		STY();
		break;

	//Transfer
	case 0x23:
		TAX();
		break;
	case 0x24:
		TXA();
		break;
	case 0x25:
		TAY();
		break;
	case 0x26:
		TYA();
		break;

	//Math
	case 0x27:
		ADC_IM();
		break;
	case 0x28:
		ADC_AB();
		break;
	case 0x29:
		ADC_ABX();
		break;
	case 0x2a:
		ADC_ABY();
		break;
	case 0x2b:
		ADC_INDX();
		break;
	case 0x2c:
		ADC_INDY();
		break;
	case 0x2d:
		SBC_IM();
		break;
	case 0x2e:
		SBC_AB();
		break;
	case 0x2f:
		SBC_ABX();
		break;
	case 0x30:
		SBC_ABY();
		break;
	case 0x31:
		SBC_INDX();
		break;
	case 0x32:
		SBC_INDY();
		break;
	case 0x33:
		INC_AB();
		break;
	case 0x34:
		INC_ABX();
		break;
	case 0x35:
		DEC_AB();
		break;
	case 0x36:
		DEC_ABX();
		break;
	case 0x37:
		INX();
		break;
	case 0x38:
		DEX();
		break;
	case 0x39:
		INY();
		break;
	case 0x3a:
		DEY();
		break;

	//shift
	case 0x3b:
		ASL_AC();
		break;
	case 0x3c:
		ASL_AB();
		break;
	case 0x3d:
		ASL_ABX();
		break;
	case 0x3e:
		LSR_AC();
		break;
	case 0x3f:
		LSR_AB();
		break;
	case 0x40:
		LSR_ABX();
		break;
	case 0x41:
		ROL_AC();
		break;
	case 0x42:
		ROL_AB();
		break;
	case 0x43:
		ROL_ABX();
		break;
	case 0x44:
		ROR_AC();
		break;
	case 0x45:
		ROR_AB();
		break;
	case 0x46:
		ROR_ABX();
		break;

	//bitwise
	case 0x47:
		AND_IM();
		break;
	case 0x48:
		AND_AB();
		break;
	case 0x49:
		AND_ABX();
		break;
	case 0x4a:
		AND_ABY();
		break;
	case 0x4b:
		AND_INDX();
		break;
	case 0x4c:
		AND_INDY();
		break;
	case 0x4d:
		ORA_IM();
		break;
	case 0x4e:
		ORA_AB();
		break;
	case 0x4f:
		ORA_ABX();
		break;
	case 0x50:
		ORA_ABY();
		break;
	case 0x51:
		ORA_INDX();
		break;
	case 0x52:
		ORA_INDY();
		break;
	case 0x53:
		EOR_IM();
		break;
	case 0x54:
		EOR_AB();
		break;
	case 0x55:
		EOR_ABX();
		break;
	case 0x56:
		EOR_ABY();
		break;
	case 0x57:
		EOR_INDX();
		break;
	case 0x58:
		EOR_INDY();
		break;
	case 0x59:
		BIT_IM();
		break;
	case 0x5a:
		BIT_AB();
		break;

	//Compare
	case 0x5b:
		CMP_IM();
		break;
	case 0x5c:
		CMP_AB();
		break;
	case 0x5d:
		CMP_ABX();
		break;
	case 0x5e:
		CMP_ABY();
		break;
	case 0x5f:
		CMP_INDX();
		break;
	case 0x60:
		CMP_INDY();
		break;
	case 0x61:
		CPX_IM();
		break;
	case 0x62:
		CPX_AB();
		break;
	case 0x63:
		CPY_IM();
		break;
	case 0x64:
		CPY_AB();
		break;

	//branch
	case 0x65:
		BCC();
		break;
	case 0x66:
		BCS();
		break;
	case 0x67:
		BEQ();
		break;
	case 0x68:
		BNE();
		break;
	case 0x69:
		BPL();
		break;
	case 0x6a:
		BMI();
		break;
	case 0x6b:
		BVC();
		break;
	case 0x6c:
		BVS();
		break;

	//jump
	case 0x6d:
		JMP_AB();
		break;
	case 0x6e:
		JMP_IN();
		break;
	case 0x6f:
		JSR();
		break;
	case 0x70:
		RTS();
		break;

	//stack
	case 0x71:
		PHA();
		break;
	case 0x72:
		PLA();
		break;
	case 0x73:
		PHP();
		break;
	case 0x74:
		PLP();
		break;
	case 0x75:
		TXS();
		break;
	case 0x76:
		TSX();
		break;

	//flags
	case 0x77:
		CLC();
		break;
	case 0x78:
		SEC();
		break;
	case 0x79:
		CLV();
		break;
	}
}

void getAddress()
{
	address = 0x0000;
	address |= getValue(program_counter);
	address = address << 8;
	program_counter++;
	address |= getValue(program_counter);
	program_counter++;
}

void getAddressIndX()
{
	address = 0x0000;
	address |= getValue(getValue(program_counter + reg_x));
	address = address << 8;
	program_counter++;
	address |= getValue(getValue(program_counter + reg_x));
	program_counter++;
}

void getAddressIndY()
{
	address = 0x0000;
	address |= getValue(getValue(program_counter));
	address = address << 8;
	program_counter++;
	address |= getValue(getValue(program_counter));
	program_counter++;
}

void pushToStack(uint8_t value)
{
	setValue(stack_counter, value);
	stack_counter++;
}

uint8_t popFromStack()
{
	stack_counter--;
	return getValue(stack_counter);
}

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

			if (!wait)
			{
				run_opcode();
			}
			else
			{
				wait--;
			}

			cpu_stage++;
			if (cpu_stage > 100)
			{
				cpu_stage = 0;
			}
		}

		end_time = clock();
	}
}

//add with carry bit (acc + value + carry bit)
int ADC_IM()
{
	uint16_t result = 0x0000;
	result = accumulator + getValue(program_counter) + (flags & 1);
	
	if (result > 0xFF)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(program_counter)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	program_counter++;
	return 1;
}

int ADC_AB()
{
	uint16_t result = 0x0000;
	getAddress();
	result = accumulator + getValue(address) + (flags & 1);

	if (result > 0xFF)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 3;
}

int ADC_ABX()
{
	uint16_t result = 0x0000;
	getAddress();
	result = accumulator + getValue(address + reg_x) + (flags & 1);

	if (result > 0xFF)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address + reg_x)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 3;
}

int ADC_ABY()
{
	uint16_t result = 0x0000;
	getAddress();
	result = accumulator + getValue(address + reg_y) + (flags & 1);

	if (result > 0xFF)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address + reg_y)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 3;
}

int ADC_INDX()
{
	uint16_t result = 0x0000;
	getAddressIndX();
	result = accumulator + getValue(address) + (flags & 1);

	if (result > 0xFF)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 5;
}

int ADC_INDY()
{
	uint16_t result = 0x0000;
	getAddressIndY();
	result = accumulator + getValue(address + reg_y) + (flags & 1);

	if (result > 0xFF)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address + reg_y)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 4;
}

//and value with accumulator
int AND_IM()
{
	accumulator = accumulator & getValue(program_counter);

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;
	return 1;
}

int AND_AB()
{
	getAddress();
	accumulator = accumulator & getValue(address);

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

int AND_ABX()
{
	getAddress();
	accumulator = accumulator & getValue(address + reg_x);

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

int AND_ABY()
{
	getAddress();
	accumulator = accumulator & getValue(address + reg_y);

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

int AND_INDX()
{
	getAddressIndX();
	accumulator = accumulator & getValue(address);

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

int AND_INDY()
{
	getAddressIndY();
	accumulator = accumulator & getValue(address + reg_y);

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

//arithmatic shift left
int ASL_AC()
{
	if (accumulator & 0x80)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	accumulator = accumulator << 1;

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

int ASL_AB()
{
	getAddress();

	if (getValue(address) & 0x80)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	setValue(address, getValue(address) << 1);

	if (getValue(address) == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (getValue(address) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

int ASL_ABX()
{
	getAddress();

	if (getValue(address + reg_x) & 0x80)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	setValue(address + reg_x, getValue(address + reg_x) << 1);

	if (getValue(address + reg_x) == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (getValue(address + reg_x) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 6;
}

//branch if carry bit is clear to program counter (at value position) + value(signed)
int BCC()
{
	if (!(flags & 0b00000001))
	{
		program_counter += (int8_t)getValue(program_counter);
		return 2;
	}
	return 1;
}

//branch if carry bit is set to program counter (at value position) + value(signed)
int BCS()
{
	if (flags & 0b00000001)
	{
		program_counter += (int8_t)getValue(program_counter);
		return 2;
	}
	return 1;
}

//branch if zero bit is set to program counter (at value position) + value(signed)
int BEQ()
{
	if (flags & 0b00000010)
	{
		program_counter += (int8_t)getValue(program_counter);
		return 2;
	}
	return 1;
}

//sets flags based on value (if bit 7 then negative is set, if bit 6 then overflow is set, if the result of value & acc is zero then zero is set)
int BIT_IM()
{
	if (getValue(program_counter) & 10000000)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	if (getValue(program_counter) & 01000000)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (!(accumulator & getValue(program_counter)))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	program_counter++;
	return 1;
}

int BIT_AB()
{
	getAddress();

	if (getValue(address) & 10000000)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	if (getValue(address) & 01000000)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (!(accumulator & getValue(address)))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	return 2;
}

//branch if negative bit is set to program counter (at value position) + value(signed)
int BMI()
{
	if (flags & 0b00001000)
	{
		program_counter += (int8_t)getValue(program_counter);
		return 2;
	}
	return 1;
}

//branch if zero bit is clear to program counter (at value position) + value(signed)
int BNE()
{
	if (!(flags & 0b00000010))
	{
		program_counter += (int8_t)getValue(program_counter);
		return 2;
	}
	return 1;
}

//branch if negative bit is clear to program counter (at value position) + value(signed)
int BPL()
{
	if (!(flags & 0b00001000))
	{
		program_counter += (int8_t)getValue(program_counter);
		return 2;
	}
	return 1;
}

//branch if overflow bit is clear to program counter (at value position) + value(signed)
int BVC()
{
	if (!(flags & 0b00000100))
	{
		program_counter += (int8_t)getValue(program_counter);
		return 2;
	}
	return 1;
}

//branch if overflow bit is clear to program counter (at value position) + value(signed)
int BVS()
{
	if (flags & 0b00000100)
	{
		program_counter += (int8_t)getValue(program_counter);
		return 2;
	}
	return 1;
}

//clear the carry flag
int CLC()
{
	flags &= 0b11111110; //carry
	return 1;
}

//clear the overflow flag
int CLV()
{
	flags &= 0b11111011; //overflow
	return 1;
}

//compares the acc to value
int CMP_IM()
{
	if (accumulator >= getValue(program_counter))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (accumulator == getValue(program_counter))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((accumulator - getValue(program_counter)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;
	return 1;
}

int CMP_AB()
{
	getAddress();

	if (accumulator >= getValue(address))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (accumulator == getValue(address))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((accumulator - getValue(address)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

int CMP_ABX()
{
	getAddress();

	if (accumulator >= getValue(address + reg_x))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (accumulator == getValue(address + reg_x))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((accumulator - getValue(address + reg_x)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

int CMP_ABY()
{
	getAddress();

	if (accumulator >= getValue(address + reg_y))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (accumulator == getValue(address + reg_y))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((accumulator - getValue(address + reg_y)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

int CMP_INDX()
{
	getAddressIndX();

	if (accumulator >= getValue(address))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (accumulator == getValue(address))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((accumulator - getValue(address)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

int CMP_INDY()
{
	getAddressIndY();

	if (accumulator >= getValue(address + reg_y))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (accumulator == getValue(address + reg_y))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((accumulator - getValue(address + reg_y)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 4;
}

//compares the reg_x to value
int CPX_IM()
{
	if (reg_x >= getValue(program_counter))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (reg_x == getValue(program_counter))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((reg_x - getValue(program_counter)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;
	return 1;
}

int CPX_AB()
{
	getAddress();

	if (reg_x >= getValue(address))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (reg_x == getValue(address))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((reg_x - getValue(address)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

//compares the reg_y to value
int CPY_IM()
{
	if (reg_y >= getValue(program_counter))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (reg_y == getValue(program_counter))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((reg_y - getValue(program_counter)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;
	return 1;
}

int CPY_AB()
{
	getAddress();

	if (reg_y >= getValue(address))
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (reg_y == getValue(address))
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((reg_y - getValue(address)) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

//Decrements address's value
DEC_AB()
{
	getAddress();

	setValue(address, getValue(address) - 1);

	if (getValue(address) == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if(getValue(address) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

DEC_ABX()
{
	getAddress();

	setValue(address, getValue(address + reg_x) - 1);

	if (getValue(address + reg_x) == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (getValue(address + reg_x) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 6;
}

//Decrements the X register
DEX()
{
	reg_x -= 1;

	if (reg_x == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_x & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//Decrements the Y register
DEY()
{
	reg_y -= 1;

	if (reg_y == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_y & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//Exclusive or
EOR_IM()
{
	accumulator = accumulator ^ getValue(program_counter);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;
	return 1;
}

EOR_AB()
{
	getAddress();
	accumulator = accumulator ^ getValue(address);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

EOR_ABX()
{
	getAddress();
	accumulator = accumulator ^ getValue(address + reg_x);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

EOR_ABY()
{
	getAddress();
	accumulator = accumulator ^ getValue(address + reg_y);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

EOR_INDX()
{
	getAddressIndX();
	accumulator = accumulator ^ getValue(address);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

EOR_INDY()
{
	getAddressIndY();
	accumulator = accumulator ^ getValue(address + reg_y);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//Increments address's value
INC_AB()
{
	getAddress();

	setValue(address, getValue(address) + 1);

	if (getValue(address) == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (getValue(address) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

INC_ABX()
{
	getAddress();

	setValue(address, getValue(address + reg_x) + 1);

	if (getValue(address + reg_x) == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (getValue(address + reg_x) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 6;
}

//Increments the X register
INX()
{
	reg_x += 1;

	if (reg_x == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_x & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//Increments the Y register
INY()
{
	reg_y += 1;

	if (reg_y == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_y & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//jump to address
JMP_AB()
{
	getAddress();

	program_counter = address;

	return 2;
}

JMP_IN()
{
	getAddress();

	program_counter = address;

	getAddress();

	program_counter = address;

	return 4;
}

//Jump to subroutine
JSR()
{
	getAddress();

	pushToStack(program_counter);
	pushToStack(program_counter >> 8);

	program_counter = address;

	return 5;
}

//Load into accumulator
LDA_IM()
{
	accumulator = getValue(program_counter);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;

	return 1;
}

LDA_AB()
{
	getAddress();
	accumulator = getValue(address);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

LDA_ABX()
{
	getAddress();
	accumulator = getValue(address + reg_x);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

LDA_ABY()
{
	getAddress();
	accumulator = getValue(address + reg_y);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

LDA_INDX()
{
	getAddressIndX();
	accumulator = getValue(address);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

LDA_INDY()
{
	getAddressIndY();
	accumulator = getValue(address + reg_y);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 4;
}

//Load into reg_x
LDX_IM()
{
	reg_x = getValue(program_counter);

	if (reg_x == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_x & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;

	return 1;
}

LDX_AB()
{
	getAddress();
	reg_x = getValue(address);

	if (reg_x == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_x & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

LDX_ABY()
{
	getAddress();
	reg_x = getValue(address + reg_y);

	if (reg_x == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_x & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

//Load into reg_y
LDY_IM()
{
	reg_y = getValue(program_counter);

	if (reg_y == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_y & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;

	return 1;
}

LDY_AB()
{
	getAddress();
	reg_y = getValue(address);

	if (reg_y == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_y & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

LDY_ABX()
{
	getAddress();
	reg_y = getValue(address + reg_x);

	if (reg_y == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_y & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

//Logical shift right
int LSR_AC()
{
	if (accumulator & 0x01)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	accumulator = accumulator >> 1;

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	flags &= 0b11110111; //negitive

	return 1;
}

int LSR_AB()
{
	getAddress();

	if (getValue(address) & 0x01)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	setValue(address, getValue(address) >> 1);

	if (getValue(address) == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	flags &= 0b11110111; //negitive

	return 5;
}

int LSR_ABX()
{
	getAddress();

	if (getValue(address + reg_x) & 0x01)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	setValue(address + reg_x, getValue(address + reg_x) << 1);

	if (getValue(address + reg_x) == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	flags &= 0b11110111; //negitive

	return 6;
}

//no process. wastes 2 cpu cycles (1 to call and 1 to wait)
int NOP()
{
	return 1;
}

//ORs the value with the acc
int ORA_IM()
{
	accumulator |= getValue(program_counter);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	program_counter++;
	return 1;
}

int ORA_AB()
{
	getAddress();
	accumulator |= getValue(address);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

int ORA_ABX()
{
	getAddress();
	accumulator |= getValue(address + reg_x);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

int ORA_ABY()
{
	getAddress();
	accumulator |= getValue(address + reg_y);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

int ORA_INDX()
{
	getAddressIndX();
	accumulator |= getValue(address);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

int ORA_INDY()
{
	getAddressIndY();
	accumulator |= getValue(address + reg_y);

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 4;
}

//Push acc
int PHA()
{
	pushToStack(accumulator);
	return 2;
}

//Push flags
int PHP()
{
	pushToStack(flags);
	return 2;
}

//pop accumulator from stack
int PLA()
{
	accumulator = popFromStack();

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 3;
}

//pop flags from stack
int PLP()
{
	flags = popFromStack();
	return 3;
}

//Rotate Left
int ROL_AC()
{
	if (accumulator & 0x80)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	accumulator = accumulator << 1;
	accumulator |= (flags & 0b00000001);

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

int ROL_AB()
{
	uint8_t result;
	getAddress();

	if (getValue(address) & 0x80)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	result = getValue(address) << 1;
	result |= (flags & 0b00000001);
	setValue(address, result);

	if (getValue(address) == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (getValue(address) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 5;
}

int ROL_ABX()
{
	uint8_t result;
	getAddress();

	if (getValue(address + reg_x) & 0x80)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	result = getValue(address + reg_x) << 1;
	result |= (flags & 0b00000001);
	setValue(address + reg_x, result);

	if (getValue(address + reg_x) == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (getValue(address + reg_x) & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 6;
}

//Rotate Right
int ROR_AC()
{
	if (accumulator & 0x01)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	accumulator = accumulator >> 1;
	if (flags & 0b00000001)
	{
		accumulator |= 0b10000000;
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	if (accumulator == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	return 1;
}

int ROR_AB()
{
	uint8_t result;
	getAddress();

	if (getValue(address) & 0x01)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	result = getValue(address) >> 1;
	if (flags & 0b00000001)
	{
		result |= 0b10000000;
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}
	setValue(address, result);

	if (getValue(address) == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	return 5;
}

int ROR_ABX()
{
	uint8_t result;
	getAddress();

	if (getValue(address + reg_x) & 0x01)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	result = getValue(address + reg_x) >> 1;
	if (flags & 0b00000001)
	{
		result |= 0b10000000;
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}
	setValue(address + reg_x, result);

	if (getValue(address + reg_x) == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	return 6;
}

//Return from subroutine
int RTS()
{
	program_counter = 0x0000;

	program_counter |= popFromStack();
	program_counter = program_counter << 8;
	program_counter |= popFromStack();
	
	return 5;
}

//subtract with carry bit (acc + value + carry bit)
int SBC_IM()
{
	uint8_t result = 0x0000;
	result = accumulator - getValue(program_counter) + (flags & 1);

	if ((accumulator - getValue(program_counter) + (flags & 1)) < 0)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(program_counter)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	program_counter++;
	return 1;
}

int SBC_AB()
{
	uint8_t result = 0x0000;
	getAddress();
	result = accumulator - getValue(address) + (flags & 1);

	if ((accumulator - getValue(address) + (flags & 1)) < 0)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 3;
}

int SBC_ABX()
{
	uint8_t result = 0x0000;
	getAddress();
	result = accumulator - getValue(address + reg_x) + (flags & 1);

	if ((accumulator - getValue(address + reg_x) + (flags & 1)) < 0)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address + reg_x)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 3;
}

int SBC_ABY()
{
	uint8_t result = 0x0000;
	getAddress();
	result = accumulator - getValue(address + reg_y) + (flags & 1);

	if ((accumulator - getValue(address + reg_y) + (flags & 1)) < 0)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address + reg_y)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 3;
}

int SBC_INDX()
{
	uint8_t result = 0x0000;
	getAddressIndX();
	result = accumulator - getValue(address) + (flags & 1);

	if (accumulator - getValue(address) + (flags & 1) < 0)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 5;
}

int SBC_INDY()
{
	uint8_t result = 0x0000;
	getAddressIndY();
	result = accumulator - getValue(address + reg_y) + (flags & 1);

	if ((accumulator - getValue(address + reg_y) + (flags & 1)) < 0)
	{
		flags |= 0b00000001; //carry
	}
	else
	{
		flags &= 0b11111110; //carry
	}

	if (result == 0x00)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if ((result ^ accumulator) & (result ^ getValue(address + reg_y)) & 0x80)
	{
		flags |= 0b00000100; //overflow
	}
	else
	{
		flags &= 0b11111011; //overflow
	}

	if (result & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	accumulator = result;
	return 4;
}

//set carry flag
int SEC()
{
	flags |= 0b00000001;
	return 1;
}

//Store accumulator in memory
int STA_AB()
{
	getAddress();
	setValue(address, accumulator);
	return 3;
}

int STA_ABX()
{
	getAddress();
	setValue(address + reg_x, accumulator);
	return 3;
}

int STA_ABY()
{
	getAddress();
	setValue(address + reg_y, accumulator);
	return 3;
}

int STA_INDX()
{
	getAddressIndX();
	setValue(address, accumulator);
	return 5;
}

int STA_INDY()
{
	getAddressIndY();
	setValue(address + reg_y, accumulator);
	return 4;
}

//store reg_x in memory
int STX()
{
	getAddress();
	setValue(address, reg_x);
	return 3;
}

//store reg_y in memory
int STY()
{
	getAddress();
	setValue(address, reg_y);
	return 3;
}

//transfer accumulator to reg_x
int TAX()
{
	reg_x = accumulator;

	if (reg_x == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_x & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//transfer accumulator to reg_y
int TAY()
{
	reg_y = accumulator;

	if (reg_y == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_y & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//transfer stack_counter to reg_x
int TSX()
{
	reg_x = stack_counter;

	if (reg_x == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (reg_x & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//transfer reg_x to accumulator
int TXA()
{
	accumulator = reg_x;

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//transfer reg_x to stack_counter
int TXS()
{
	stack_counter = reg_x;

	if (stack_counter == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (stack_counter & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}

//transfer reg_y to accumulator
int TYA()
{
	accumulator = reg_y;

	if (accumulator == 0)
	{
		flags |= 0b00000010; //zero
	}
	else
	{
		flags &= 0b11111101; //zero
	}

	if (accumulator & 0x80)
	{
		flags |= 0b00001000; //negative
	}
	else
	{
		flags &= 0b11110111; //negative
	}

	return 1;
}