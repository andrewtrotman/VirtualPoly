/*
	MC6854.H
	--------
	Copyright (c) 2025 Andrew Trotman
*/
#pragma once

#include <stdio.h>
#include "mc6854_channel.h"

/*
	class MC6854
	------------
*/
class mc6854
{
public:
	static const unsigned short flag_address = 0x100;
	static const unsigned short flag_eot = 0x200;
	static const unsigned short flag_data = 0x400;

public:
	mc6854_channel *channel_out;
	mc6854_channel *channel_in;

public:
	// control register 1
	long ac;						// AC		(which register is at 2/3 and 4/ft)
	long r_interrupt;			// RIE
	long t_interrupt;			// TIE
	long rdsr;					// RDSR
	long tdsr;					// TDSR
	long discontinue;			// DISCONTINUE (ditch current frame)
	long r_reset;				// RxRS
	long t_reset;				// TxRS
	// control register 2
	long pse;					// PSE - Prioritised Status Enable
	long byte21;				// 2/1 BYTE
	long fm_idle;				// F/M IDLE
	long frame_complete;		// TC/TDRA
	long rts;					// RTS
	// control register 3
	long lcf;					// LCF
	long c_ex;					// Cex
	long a_ex;					// Aex
	long idle_01_11;			// 01/11 IDLE
	long fdse;					// FDSE
	long loop;					// LOOP
	long gap;					// GAP/TEST	(go active on poll)
	long online;				// LOC/DTR
	// control register 4
	long ff_f;					// DOUBLE FLAG / SINGLE FLAG INTERFRAME
	long tx_word_len;			// TxWLS1 + TxWLS2
	long rx_word_len;			// RxWLS1 + RxWLS2
	long transmit_abort;		// ABT
	long abort_ex;				// ABTex
	long nrzi;					// NRZI

protected:
	void control2(unsigned char val);
	void control3(unsigned char val);
	void control4(unsigned char val);

	void control1(unsigned char val);
	void control23(unsigned char val);
	void control4ft(unsigned char val);
	unsigned char status1(void);
	unsigned char status2(void);

	unsigned char read_byte(void);
	void write_byte(unsigned char val);

public:
	mc6854();
	virtual ~mc6854() {}

	void write(unsigned short address, unsigned char value);
	unsigned char read(unsigned short address);

	void set_outstream(mc6854 *up) { channel_out = up->channel_in; }
	
	virtual bool is_signaling_irq(void) { return channel_in->peek() & r_interrupt; }
} ;

