/*
	MC6854.C
	--------
	Copyright (c) 2025 Andrew Trotman
*/
#include <string.h>
#include <stdio.h>
#include "mc6854.h"

/*
	MC6854::MC6854()
	----------------
*/
mc6854::mc6854(computer_proteus *machine)
{
/*
	MC6854 registers
*/
ac = r_interrupt = t_interrupt = rdsr = tdsr = r_reset = t_reset = 0;
pse = byte21 = fm_idle = frame_complete = rts = 0;
lcf = c_ex = a_ex = idle_01_11 = fdse = loop = gap = online = 0;
ff_f = tx_word_len = rx_word_len = transmit_abort = abort_ex = nrzi = 0;
discontinue = 0;

channel_out = channel_in = new mc6854_channel(machine);
channel_up = NULL;					// upstream to the next poly
}

/*
	MC6854::CONTROL1()
	------------------
*/
void mc6854::control1(unsigned char val)
{
ac = val & 0x01;
r_interrupt = val & 0x02;
t_interrupt = val & 0x04;
rdsr = val & 0x08;
tdsr = val & 0x10;
discontinue = val & 0x20;
r_reset = val & 0x40;
t_reset = val & 0x80;

if (discontinue)
	discontinue = channel_in->discontinue();
}

/*
	MC6854::CONTROL2()
	------------------
*/
void mc6854::control2(unsigned char val)
{
pse = val & 0x01;
byte21 = val & 0x02;
fm_idle = val & 0x04;
frame_complete = val & 0x08;

if (val & 0x10) // transmit last data
	channel_out->eot();

if (val & 0x20) 
	{
	/*
		clear reciever status
	*/
	}
if (val & 0x40) 
	{
	/*
		clear transmit status
	*/
	}

/* 
	RTS is used by the Poly to effect loop-back control
		RTS disabled = transmitter sends message back towards server
		RTS enabled = transmitter sends message to next Poly in chain (possibly falling off the chain)
*/
rts = val & 0x80;
}


/*
	MC6854::CONTROL3()
	------------------
*/
void mc6854::control3(unsigned char val)
{
lcf = val & 0x01;
c_ex = val & 0x02;
a_ex = val & 0x04;
idle_01_11 = val & 0x08;
fdse = val & 0x10;
loop = val & 0x20;
gap = val & 0x40;
online = val & 0x80;
}

/*
	MC6854::CONTROL23()
	-------------------
*/
void mc6854::control23(unsigned char val)
{
if (ac == 0)
	control2(val);
else
	control3(val);
}

/*
	MC6854::CONTROL4()
	------------------
*/
void mc6854::control4(unsigned char val)
{
ff_f = val & 0x01;
tx_word_len = ((val >> 1) & 0x03) + 5;
rx_word_len = ((val >> 3) & 0x03) + 5;
transmit_abort = val & 0x20;
abort_ex = val & 0x40;
nrzi = val & 0x80;
}

/*
	MC6854::CONTROL4FT()
	--------------------
*/
void mc6854::control4ft(unsigned char val)
{
if (ac)
	control4(val);
else
	{
	write_byte(val);
	channel_out->eot();
	}
}

/*
	MC6854::STATUS1()
	-----------------
	80: Interrupt Request
	40: Transmitter Data Register Available / Frame Complete
	20: Transmitter Underrun
	10: Clear To Send
	08: Flag Detected
	04: Loop Status (see control register 3)
	02: Status Register 2 Read Request (most of status2)
	01: Recieve Data Available (also in status2)
*/
unsigned char mc6854::status1(void)
{
unsigned long ans;

ans = 0x00;
if (channel_in->peek())
	ans |= 0x80; 			// interrupt request

ans |= 0x40; 			// TDRA / FC
ans |= 0x00; 			// underrun
ans |= 0x00; 			// CTS
ans |= 0x00; 			// Flag Detected
if (loop && online)
	ans |= 0x04; 		// Loop status

if (status2() & 0x7F)		// status register 2 needs to be read (address | end frame present)
	ans |= 0x02; 		// status register 2 read request

if (status2() >> 7)
	ans |= 0x01; 		// RDA (also in status register 2)

return (unsigned char) ans;
}

/*
	MC6854::STATUS2()
	-----------------
	80: Recieve Data Available (also in status1)
	40: Reciever Overrun
	20: Data Carrier Detect
	10: Error
	08: RxAbort
	04: RxIdle
	02: Frame Valid
	01: Address Present
*/
unsigned char mc6854::status2(void)
{
unsigned char ans;
unsigned short next_byte;

ans = 0x00;

next_byte = channel_in->peek();

if (next_byte)
	{
	if (pse)
		{
		if (next_byte & flag_data)
			ans |= 0x80;
		else
			ans |= 0x00;
		}
	else
		ans |= 0x80;				// something ready to be read
	}

ans |= 0x00; 		// overrun
ans |= 0x00; 		// carrier detect
ans |= 0x00; 		// error

ans |= 0x00; 		// RxAbort
ans |= 0x00; 		// RxIdle

if (next_byte & flag_eot)
	ans |= 0x02;
if (next_byte & flag_address)
	ans |= 0x01;

return ans;
}

/*
	MC6854::WRITE_BYTE()
	--------------------
*/
void mc6854::write_byte(unsigned char val)
{
channel_out->send(val);
}

/*
	MC6854::READ_BYTE()
	-------------------
*/
unsigned char mc6854::read_byte(void)
{
unsigned short in_value;

in_value = channel_in->recieve();

return (unsigned char)(in_value & 0xFF);
}

/*
	MC6854::WRITE()
	---------------
*/
void mc6854::write(unsigned short address, unsigned char value)
{
switch(address)
	{
	case 0:
		control1(value);
		break;

	case 1:
		control23(value);
		break;

	case 2:
		write_byte(value);
		break;

	case 3:
		control4ft(value);
		break;
	}
}

/*
	MC6854::READ()
	--------------
*/
unsigned char mc6854::read(unsigned short address)
{
switch (address)
	{
	case 0x0000:
		return status1();

	case 0x0001:
		return status2();

	case 0x0002:
		return read_byte();

	case 0x0003:
		return read_byte();

	default:
		return 0;
	}
}

