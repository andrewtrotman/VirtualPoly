/*
	COMPUTER_ARROW.H
	----------------
	Copyright (c) 2021 Andrew Trotman
*/
#pragma once

#include "computer.h"
#include "keyboard_arrow.h"

/*
	CLASS COMPUTER_ARROW
	--------------------
*/
class computer_arrow : public computer
	{
	private:
		keyboard_arrow keyboard;
		
	public:
		computer_arrow();
		virtual ~computer_arrow();

		virtual byte read(word offset);
		virtual void write(word offset, byte val);

		virtual const uint8_t *screen_buffer(void);
		virtual void queue_key_press(byte key);
	};
