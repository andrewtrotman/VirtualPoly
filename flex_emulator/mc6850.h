/*
	MC6850.H
	--------
	Copyright (c) 2021 Andrew Trotman
*/
#pragma once

#include <deque>
#include <iostream>

#include "typedefs.h"

/*
	CLASS MC6850
	------------
	Motorola 6850 ACIA (Serial Port)
*/
class mc6850
	{
	friend inline std::ostream &operator<<(std::ostream &into, const mc6850 &simulator);
	friend inline std::istream &operator>>(std::istream &from, mc6850 &simulator);

	private:
		byte baud_rate_register;
		std::deque<byte> &input;
		std::deque<byte> &output;

	protected:
		virtual void reset(void);
		virtual byte poll(void);
		virtual byte in(void);
		virtual void out(byte value);

	public:
		mc6850(std::deque<byte> &input, std::deque<byte> &output) :
			baud_rate_register(0),
			input(input),
			output(output)
			{
			reset();
			}

		virtual ~mc6850()
			{
			/* Nothing */
			}

		byte read(word address);
		void write(word address, byte value);
	};


/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const mc6850 &simulator)
	{
	into.write((char *)&simulator.baud_rate_register, sizeof(simulator.baud_rate_register));

	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, mc6850 &simulator)
	{
	from.read((char *)&simulator.baud_rate_register, sizeof(simulator.baud_rate_register));

	return from;
	}
