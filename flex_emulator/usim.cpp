/*
	USIM.CPP
	--------
	Based on original (c) 1994 R.P.Bellis
	Re-factoring (c) 2010 Andrew Trotman
	Ported to Mac 2021 Andrew Trotman
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "usim.h"

/*
	USIM::RUN()
	-----------
*/
void USim::run(void)
	{
	halted = 0;
	while (!halted)
		step();
	}

/*
	USIM::STEP()
	------------
*/
void USim::step(void)
	{
	execute();
	}

/*
	USIM::HALT()
	------------
*/
void USim::halt(void)
	{
	halted = 1;
	}

/*
	USIM::FETCH()
	-------------
*/
byte USim::fetch(void)
	{
	return read(pc++);
	}

/*
	USIM::FETCH_WORD()
	------------------
*/
word USim::fetch_word(void)
	{
	word val = read_word(pc);
	pc += 2;

	return val;
	}

/*
	USIM::INVALID()
	---------------
*/
void USim::invalid(const char *msg)
	{
	fprintf(stderr, "Invalid %s : pc = [%04x], ir = [%04x]\n", msg ? msg : "", pc, ir);
//	halt();
	}

/*
	USIM::READ()
	------------
*/
byte USim::read(word offset)
	{
	return memory[offset];
	}

/*
	USIM::WRITE()
	-------------
*/
void USim::write(word offset, byte val)
	{
	memory[offset] = val;
	}

/*
	USIM::READ_WORD()
	-----------------
*/
word USim::read_word(word offset)
	{
	word tmp;

	tmp = read(offset++) << 8;
	tmp |= read(offset);

	return tmp;
	}

/*
	USIM::WRITE_WORD()
	------------------
*/
void USim::write_word(word offset, word val)
	{
	write(offset++, (byte)(val >> 8));
	write(offset, (byte)val);
	}
