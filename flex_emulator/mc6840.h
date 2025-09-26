/*
	MC6840.H
	--------
*/
#pragma once

#include "typedefs.h"

/*
	MC6840
	------
	Motorola 6840  Programable Timer
*/
class mc6840
{
protected:
	long clear_on_read;

	byte msb_latch, lsb_latch;
	word timer_1_state;
	word timer_1_rate, timer_2_rate, timer_3_rate;
	word timer_1_control, timer_2_control, timer_3_control;
	word timer_1_counter, timer_2_counter, timer_3_counter; 
	byte status_register;

protected:
	void compute_irq(void);

public:
	mc6840();
	virtual ~mc6840() {}

	void step(void);

	virtual byte read(word address);
	virtual void write(word address, byte value);
	virtual void reset(void);

	virtual bool is_signaling_irq(void) { return status_register & 0x80; }

} ;
