/*
	USIM.H
	------
	Based on original (c) 1994 R.P.Bellis
	Substantial re-factoring (c) 2010 Andrew Trotman
	Ported to Mac 2021 Andrew Trotman
*/
#ifndef __usim_h__
#define __usim_h__

#include <deque>

#include "misc.h"

/*
	class USim
	----------
*/
class USim
{
public:
	int halted;					// are we in the halted state?
	byte memory[0x10000];	// memory
	word ir, pc;				// internal register and program counter

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
	virtual void step(void);
	virtual void halt(void);
	virtual void reset(void) = 0;
	virtual void invalid(const char * = 0);

	virtual void queue_key_press(byte key) = 0;
	virtual word dequeue_serial_output(void) = 0;
} ;

#endif // __usim_h__
