/*
	USIM.H
	------
	Based on original (c) 1994 R.P.Bellis
	Re-factoring (c) 2010 Andrew Trotman
	Ported to Mac 2021 Andrew Trotman
*/
#pragma once

#include <deque>
#include <iostream>

#include "misc.h"

/*
	CLASS USIM
	----------
*/
class USim
{
public:
	int halted;					// are we in the halted state?
	byte memory[0x200000];		// memory (2MB)
	word ir, pc;				// internal register and program counter
	word start_of_instruction;	// the address of the first byte of the current instruction

protected:
	virtual byte read(word offset);
	virtual void write(word offset, byte val);
	virtual word read_word(word offset);
	virtual void write_word(word offset, word val);
	virtual byte fetch(void);
	virtual word fetch_word(void);
	virtual void execute(void) = 0;

public:
	virtual void run(void);
	virtual void step(uint64_t times);
	virtual void halt(void);
	virtual void reset(void) = 0;
	virtual void invalid(const char * = 0);

	virtual void queue_key_press(byte key) = 0;
	virtual word dequeue_serial_output(void) = 0;
} ;

/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const USim &simulator)
	{
	into.write((char *)&simulator.memory[0], sizeof(simulator.memory));
	into.write((char *)&simulator.halted, sizeof(simulator.halted));
	into.write((char *)&simulator.ir, sizeof(simulator.ir));
	into.write((char *)&simulator.pc, sizeof(simulator.pc));

	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, USim &simulator)
	{
	from.read((char *)&simulator.memory[0], sizeof(simulator.memory));
	from.read((char *)&simulator.halted, sizeof(simulator.halted));
	from.read((char *)&simulator.ir, sizeof(simulator.ir));
	from.read((char *)&simulator.pc, sizeof(simulator.pc));

	return from;
	}
