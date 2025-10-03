/*
	MC6854_CHANNEL.C
	----------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "proteus.h"
#include "mc6854_channel.h"

//#define DEBUG 1

long mc6854_channel_logging = false;
unsigned char log_buffer[1024 * 1024];
unsigned char *log_buffer_pos = log_buffer;


/*
	LOG_DECODE_MESSAGE()
	--------------------
*/
static void log_decode_message(FILE *out, long direction, unsigned char *message, unsigned char *end)
{
if (direction)
	fprintf(out, "TELL ");
else
	fprintf(out, "HEAR ");
	
if (*message == 0)
	fprintf(out, "ALL:");
else
	fprintf(out, "%02X :", *message);

switch (message[1])
	{
	case 0x13:
		if (direction)
			fprintf(out, "EstablishLoopback (as you're end of ring)");
		else
			{
			if (end - message > 2)
				fprintf(out, "MyROMid is %02X %02X", message[2], message[3]);
			else
				fprintf(out, "You're message Looped Back");
			}
		break;
	case 0x17:
		fprintf(out, "RingShutdown, EstablishLoopback");
		break;
	case 0x73:
		fprintf(out, "EstablishRing, You're ");
		if (message[2] == 0)
			fprintf(out, "Not Logged In");
		else if (message[2] == 1)
			fprintf(out, "Logged on as Standalone");
		else if (message[2] == 2)
			fprintf(out, "Logged on as online");
		else
			fprintf(out, "In an unknown state");
		break;
	case 0xBF:
		fprintf(out, "SetID to %d", message[2]);
		break;
	default:
		break;
	}
}

/*
	LOG()
	-----
*/
static void log(mc6854_channel *object, long direction, unsigned short byte)
{
FILE *fp;
static long last_direction = -1;

if (mc6854_channel_logging)
	{
	fp = fopen("log.txt", "a+");
	if (direction != last_direction)
		{
		log_buffer_pos = log_buffer;
		fprintf(fp, "%s ", direction ? "->" : "\t<-");
		last_direction = direction;
		}

	fprintf(fp, "%02x ", byte & 0xFF);
//	fprintf(fp, "%02x %c ", byte & 0xFF, isalnum(byte & 0xFF) ? byte & 0xFF : '.');
//	fprintf(fp, "%c", isprint(byte & 0xFF) ? byte & 0xFF : '.');
	*log_buffer_pos++ = (unsigned char)(byte & 0xFF);

	if (byte & mc6854::flag_eot)
		{
		fprintf(fp, "\t\t\t\t ; ");
		log_decode_message(fp, last_direction, log_buffer, log_buffer_pos);
		fprintf(fp, "\n");
		last_direction = -1;
		log_buffer_pos = log_buffer;
		}

	fclose(fp);
	}
}

/*
	MC6854_CHANNEL()
	----------------
*/
mc6854_channel::mc6854_channel(proteus *reciever)
{ 
read = write = buffer = new unsigned short[SIZE];
buffer_end = buffer + SIZE;
this->reciever = reciever;
write_fifo_pos = 0;
memset(write_fifo, 0, sizeof(write_fifo));
}

/*
	MC6845_CHANNEL::PLACE_ON_WIRE()
	-------------------------------
*/
void mc6854_channel::place_on_wire(unsigned short val)
{
#ifdef DEBUG
	log(this, true, val);
#endif

*write++ = val;

if (write >= buffer_end)
	write = buffer;

if (reciever != NULL)
	reciever->network_irq();
}

/*
	MC6854_CHANNEL::SEND()
	----------------------
*/
void mc6854_channel::send(unsigned short val)
{
if (write_fifo_pos == 0)
	val |= mc6854::flag_address;

if (write_fifo_pos < 3)
	write_fifo[write_fifo_pos++] = val;
else
	{
	place_on_wire(*write_fifo);

	write_fifo[0] = write_fifo[1];
	write_fifo[1] = write_fifo[2];
	write_fifo[2] = val;
	}
}

/*
	MC6854_CHANNEL::EOT()
	---------------------
*/
void mc6854_channel::eot(void)
{
long pos;

if (write_fifo_pos != 0)
	{
	write_fifo[write_fifo_pos - 1] |= mc6854::flag_eot;
	for (pos = 0; pos < write_fifo_pos; pos++)
		place_on_wire(write_fifo[pos]);
	write_fifo_pos = 0;
	}
}

/*
	MC6854_CHANNEL::RECIEVE()
	-------------------------
*/
unsigned short mc6854_channel::recieve(void)
{
unsigned short answer;

if (read == write)
	return 0;

answer = *read;
read++;
if (read >= buffer_end)
	read = buffer;

#ifdef DEBUG
	log(this, false, answer);
#endif

if (read == write)
	if (reciever != NULL) 
		reciever->network_d_irq();

return answer;
}

/*
	MC6854_CHANNEL::PEEK()
	----------------------
*/
unsigned short mc6854_channel::peek(void)
{
if (read == write)
	return 0;
else if (*read & 0xFF00)
	return *read;
else
	return *read | mc6854::flag_data;
}

/*
	MC6854_CHANNEL::LAST_WRITE()
	----------------------------
*/
unsigned short mc6854_channel::last_write(void)
{
if (write == buffer)
	return *(buffer_end - 1);
else
	return *(write - 1);
}

/*
	MC6854_CHANNEL::RESEND()
	------------------------
*/
void mc6854_channel::resend(unsigned short value)
{
place_on_wire(value);
}

/*
	MC6854_CHANNEL::DISCONTINUE()
	-----------------------------
*/
unsigned char mc6854_channel::discontinue(void)
{
unsigned short current;

while (read != write)
	{
	current = *read++;
	if (read >= buffer_end)
		read = buffer;
	if (current & mc6854::flag_eot)
		return 0;
	}

return 0x20;
}
