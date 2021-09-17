/*
	KEYBOARD_ARROW.H
	----------------
*/
#pragma once

#include <stdint.h>

#include <deque>

#include "typedefs.h"

extern word start_of_instruction;

/*
	CLASS KEYBOARD_ARROW
	--------------------
*/
class keyboard_arrow
	{
	private:
		std::deque<byte> &keystream;
		bool announce;
		bool reset;
		uint8_t bit_check;
		
	public:
		keyboard_arrow(std::deque<byte> &keystream);
		virtual ~keyboard_arrow();

		virtual uint8_t read(uint16_t address);
		virtual void write(uint16_t address, uint8_t value);

		virtual void queue_key_press(byte key);
	};
