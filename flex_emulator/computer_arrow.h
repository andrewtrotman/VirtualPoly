/*
	COMPUTER_ARROW.H
	----------------
	Copyright (c) 2021 Andrew Trotman
*/
#pragma once

#include "computer.h"

/*
	CLASS COMPUTER_ARROW
	--------------------
*/
class computer_arrow : public computer
	{
	public:
		computer_arrow();
		virtual ~computer_arrow();

		virtual byte read(word offset);
		virtual void write(word offset, byte val);
	};
