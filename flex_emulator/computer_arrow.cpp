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
	computer(),
	keyboard(keyboard_input)
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
	COMPUTER_ARROW::QUEUE_KEY_PRESS()
	---------------------------------
*/
void computer_arrow::queue_key_press(byte key)
	{
	keyboard.queue_key_press(key);
	}

/*
	COMPUTER_ARROW::READ()
	----------------------
	0000 - 7FFF RAM
	E008 & E00C Keyboard Controller
	E800 - EFFF Screen RAM
	F000 - FFFF ROM ("Menu")
*/
byte computer_arrow::read(word address)
	{
	byte answer;

	/*
		Keyboard controller
	*/
	if (address == 0xE008 || address == 0xE00C)
		answer = keyboard.read(address - 0xE008);
	/*
		Screen RAM, Physical RAM, and ROM
	*/
	else
		answer = memory[address];

	return answer;
	}

/*
	COMPUTER_ARROW::WRITE()
	-----------------------
	0000 - 7FFF RAM
	E008 & E00C Keyboard Controller
	E800 - EFFF Screen RAM
	F000 - FFFF ROM ("Menu")
*/
void computer_arrow::write(word address, byte value)
	{
	/*
		Physical RAM
	*/
	if (address < 0x8000)
		memory[address] = value;
	/*
		Keyboard controller
	*/
	else if (address == 0xE008 || address == 0xE00C)
		keyboard.write(address - 0xE008, value);
	/*
		Screen RAM
	*/
	else if (address >= 0xE000 && address < 0xF000)
		memory[address] = value;
	/*
		ROM	 (cannot write to ROM)
	*/
	}
