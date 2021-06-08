/*
	MC6850.CPP
	----------
*/
#include <stdio.h>
#include "mc6850.h"

/*
	MC6850::RESET()
	---------------
*/
void mc6850::reset(void)
{
}

/*
	MC6850::IN()
	------------
*/
byte mc6850::in(void)
{
#ifdef _WIN32
	return _getch();
#else
	return 0;
#endif
}

/*
	MC6850::POLL()
	--------------
*/
byte mc6850::poll(void)
{
#ifdef _WIN32
	return _kbhit();
#else
	return 0;
#endif
}

/*
	MC6850::OUT()
	-------------
*/
void mc6850::out(byte value)
{
#ifdef _WIN32
	_putch(value);
#endif
}

/*
	MC6850::READ()
	--------------
*/
byte mc6850::read(word address)
{
byte answer;

if (address == 0x0000)			// status register
	if (poll())
		answer = 0x03;		// data to read
	else
		answer = 0x02;		// no data to read
else if (address == 0x0001)
	if (poll())
		answer = in();
	else
		answer = 0x00;
else if (address == 0x0002)			// baud rate register
	answer = baud_rate_register;
else
	answer = 0;				// this case cannot happen

return answer;
}

/*
	MC6850::WRITE()
	---------------
*/
void mc6850::write(word address, byte value)
{
if (address == 0x0000)		/* 6850 Control register */
	{
	if ((value & 0x03) == 0x03)
		reset();
	}
else if (address == 0x0001)		/* 6850 data out port */
	out(value);
else if (address == 0x0002)		/* baud rate register */
	baud_rate_register = value;
}
