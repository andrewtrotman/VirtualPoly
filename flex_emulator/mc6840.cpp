/*
	MC6840.CPP
	----------
	On the Proteus it appears as though
		C2 and C3 clock at 1MHz
		O3 is connected to the 6854
		O2 is connected to C1
		O1 is floating
	On the Poly
		O3 is connected to the beeper (as is PA7 of the Video PIA)
		O2 is connected to C1
		O1 does this float?
		Timer 1 programmed (rate:0x0063)(control:0x40)
		Timer 2 programmed (rate:0x139f)(control:0x82)
		Timer 3 programmed (rate;0xffff)(control:0000)
*/
#include "mc6840.h"

/*
	MC6840::MC6840()
	----------------
*/
mc6840::mc6840()
{
reset();
}

/*
	MC6840::RESET()
	---------------
*/
void mc6840::reset(void)
{
timer_1_state = 0;			// timer1 is a square wave, either high or low
status_register = msb_latch = lsb_latch = 0;
timer_1_rate = timer_2_rate = timer_3_rate = 0xFFFF;
timer_1_counter = timer_2_counter = timer_3_counter = 0xFFFF;
timer_1_control = timer_2_control = timer_3_control = 0;
clear_on_read = false;
}

/*
	MC6840::COMPUTE_IRQ()
	---------------------
*/
void mc6840::compute_irq(void)
{
/*
	If we've already signaled an IRQ then don't do it again
*/
if (status_register & 0x80)
	return;

/*
	Check to see if we need to IRQ or not
*/
if ((timer_1_control & 0x40) && (status_register & 0x01))
	status_register |= 0x80;

if ((timer_2_control & 0x40) && (status_register & 0x02))
	status_register |= 0x80;

if ((timer_3_control & 0x40) && (status_register & 0x04))
	status_register |= 0x80;
}

/*
	MC6840::STEP()
	--------------
*/
void mc6840::step(void)
{
long do_irq = false;

if (timer_1_control & 0x01)			// all held in preset-state not operating
	return;

if (timer_2_counter == 0)
	timer_2_counter = timer_2_rate + 1;
if (timer_3_counter == 0)
	timer_3_counter = timer_3_rate + 1;

timer_2_counter--;
timer_3_counter--;

if (timer_2_counter == 0)
	{
	/*
		On the Poly the output of timer 2 is the input to timer 1.
	*/
	timer_1_state = !timer_1_state;

	if (timer_1_state)
		{
		if (timer_1_counter == 0)
			timer_1_counter = timer_1_rate + 1;
		timer_1_counter--;
		if (timer_1_counter == 0)
			{
			status_register |= 0x01;
			do_irq = true;
			}
		}
	}

if (timer_2_counter == 0)
	{
	status_register |= 0x02;
	do_irq = true;
	}

if (timer_3_counter == 0)
	{
	status_register |= 0x04;
	do_irq = true;
	}

if (do_irq)
	compute_irq();
}

/*
	READ()
	------
*/
byte mc6840::read(word address)
{
switch (address)
	{
	case 0x0000:
		return 0;

	case 0x0001:
		if (status_register & 0x80)
			clear_on_read = true;
		else
			clear_on_read = false;

		return status_register;

	case 0x0002:
		if (clear_on_read)
			status_register &= 0x7E;
		lsb_latch = timer_1_counter & 0xFF;
		return (byte)(timer_1_counter >> 8);

	case 0x0003:
		return lsb_latch;

	case 0x0004:
		if (clear_on_read)
			status_register &= 0x7D;
		lsb_latch = timer_2_counter & 0xFF;
		return (byte)(timer_2_counter >> 8);

	case 0x0005:
		return lsb_latch;

	case 0x0006:
		if (clear_on_read)
			status_register &= 0x7B;
		lsb_latch = timer_3_counter & 0xFF;
		return (byte)(timer_3_counter >> 8);

	case 0x0007:
		return lsb_latch;
	}
return 0;
}

/*
	MC6840::WRITE()
	---------------
*/
void mc6840::write(word address, byte value)
{
switch (address)
	{
	case 0x0000:
		if (timer_2_control & 0x01)
			{
			timer_1_control = value;
			if (timer_1_control & 0x01)		// registers in reset state
				status_register = 0;
			}
		else
			timer_3_control = value;
		break;

	case 0x0001:
		timer_2_control = value;
		break;

	case 0x0002:
		msb_latch = value;
		break;

	case 0x0003:
		status_register &= 0xFE;
		timer_1_counter = timer_1_rate = (msb_latch << 8) | value;
		break;

	case 0x0004:
		msb_latch = value;
		break;

	case 0x0005:
		status_register &= 0xFD;
		timer_2_counter = timer_2_rate = (msb_latch << 8) | value;
		break;

	case 0x0006:
		msb_latch = value;
		break;

	case 0x0007:
		status_register &= 0xFB;
		timer_3_counter = timer_3_rate = (msb_latch << 8) | value;
		break;
	}
}
