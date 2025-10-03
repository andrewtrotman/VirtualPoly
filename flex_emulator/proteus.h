/*
	PROTEUS.H
	---------
*/
#pragma once

#include <deque>

#include "computer.h"
#include "mc6821.h"
#include "mc6840.h"
#include "mc6850.h"
#include "mc6854.h"
#include "wd1771.h"

/*
	PROTEUS
	-------
*/
class proteus : public computer
{
public:
	enum { CPU_6809, CPU_Z80 };

public:
	long cpu_active;					// is the 6809 or the Z80 active?
	long cpu_frequency;				// what is the frequency of the CPU?
	std::deque<uint8_t> acia1_in, acia1_out;
	mc6850 acia1;						// RS232 Terminal
	std::deque<uint8_t> acia2_in, acia2_out;
	mc6850 acia2;						// RS232 Printer
	std::deque<uint8_t> acia3_in, acia3_out;
	mc6850 acia3;						// RS232 Modem
	mc6821 pia;							// Parallel port (apparently this can be a printer or a hard disk drive!)
	mc6840 timer;						// System Clock
//	mc6854 network;					// Communicaitons to the Poly network
	wd1771 fdc[4];						// Floppy disk drives
	unsigned char drive_select;	// Which disk drive is selected

	unsigned char *current_rom;					// 6809 ROM (at address $F000-$FFFF when in 6809 mode)
	unsigned char z80_eprom_shadow[0x1000];	//  Z80 RAM (at address $F000-$FFFF when in 6809 mode)

public:
	proteus();
	~proteus() {}

	virtual byte read(word address);
	virtual void write(word address, byte val);

	virtual void step(uint64_t times);
	virtual void reset(void);

	virtual void network_irq(void) {}
	virtual void network_d_irq(void) {}

	virtual unsigned char *load_rom(long version);
} ;
