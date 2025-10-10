/*
	MC6854_CHANNEL.H
	----------------
	Copyright (c) 2025 Andrew Trotman
*/
#pragma once

class computer;

/*
	class MC6854_CHANNEL
	--------------------
*/
class mc6854_channel
{
private:
	static const long SIZE = (1024 * 1024);

private:
	unsigned short *buffer;
	unsigned short *buffer_end;
	unsigned short *write;
	unsigned short *read;

	computer *reciever;

	unsigned short write_fifo[3];		// 3 byte write_fifo
	long write_fifo_pos;

private:
	void place_on_wire(unsigned short val);

public:
	mc6854_channel(computer *reciever);
	virtual  ~mc6854_channel() {}

	void send(unsigned short val);
	void eot(void);
	unsigned short recieve(void);
	unsigned short peek(void);
	unsigned short last_write(void);
	void resend(unsigned short value);
	unsigned char discontinue(void);
} ;
