/*
	KEYBOARD_ARROW.H
	----------------
*/
#pragma once

#include <stdint.h>

#include <deque>

#include "typedefs.h"

/*
	CLASS KEYBOARD_ARROW
	--------------------
*/
class keyboard_arrow
	{
	enum keyboard_mode
		{
		RESET,
		ANNOUNCE,
		REPORT
		};

	private:
		std::deque<byte> keystream;
		keyboard_mode mode;
		uint8_t bit_check;
		uint8_t where_in_sequence;

	public:
		keyboard_arrow();
		virtual ~keyboard_arrow();

		virtual uint8_t read(uint16_t address);
		virtual void write(uint16_t address, uint8_t value);

		virtual void queue_key_press(byte key);
	};
