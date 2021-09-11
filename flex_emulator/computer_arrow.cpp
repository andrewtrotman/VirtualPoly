/*
	COMPUTER_ARROW.CPP
	------------------
	Copyright (c) 2021 Andrew Trotman
*/
#include "computer_arrow.h"
#include "ROM_arrow_menu.h"

/*
	COMPUTER_ARROW::COMPUTER_ARROW()
	--------------------------------
*/
computer_arrow::computer_arrow() :
	computer()
	{
	memcpy(memory + 0xF000, ROM_arrow_menu, 0x1000);
	}

/*
	COMPUTER_ARROW::~COMPUTER_ARROW()
	---------------------------------
*/
computer_arrow::~computer_arrow()
	{
	/*	Nothing */
	}

/*
	COMPUTER_ARROW::SCREEN_BUFFER()
	-------------------------------
*/
const uint8_t *computer_arrow::screen_buffer(void)
	{
	return memory + 0xE800;
	}

/*
	COMPUTER_ARROW::READ()
	----------------------
	E000-E7FF Screen RAM				// appears to be at E800
	F000-FFFF ROM ("Menu")

	E008
*/
byte computer_arrow::read(word address)
	{
	byte answer;

	answer = memory[address];

//	if (address >= 0xE000 && address < 0xE800)
//		int x = 0;

	return answer;
	}

/*
	COMPUTER_ARROW::WRITE()
	-----------------------
	E000-E7FF Screen RAM				// appears to be at E800
	F000-FFFF ROM ("Menu")
*/
void computer_arrow::write(word address, byte value)
	{
	/*
		Screen RAM
	*/
	if (address >= 0xE000 && address < 0xF000)
		memory[address] = value;
	/*
		Physical RAM
	*/
	else if (address < 0x8000)
			memory[address] = value;
	}
