/*
	COMPUTER.H
	----------
	Copyright (c) 2021 Andrew Trotman
*/
#pragma once

#include "ide.h"
#include "mc6809.h"
#include "mc6821.h"
#include "mc6850.h"
#include "saa5050_font.h"
#include "saa5055_font.h"

/*
	CLASS COMPUTER
	--------------
*/
class computer : public mc6809
	{
	friend std::ostream &operator<<(std::ostream &into, const computer &simulator);
	friend std::istream &operator>>(std::istream &from, computer &simulator);

	protected:
		std::deque<byte> keyboard_input;
		std::deque<byte> serial_output;
		mc6850 terminal;
		mc6821 printer;
		bool prot;					// protected (BIOS) mode?
		ide hard_drive;
		std::string disk_name;
		bool screen_changed;
		uint8_t bios[0x10000];		// the BIOS is in a seperate space from the RAM

	private:
		std::string get_serialised_filename();

	public:
		computer();
		virtual ~computer();

		virtual void serialise(void);
		virtual void deserialise(void);

		virtual byte read(word offset);
		virtual void write(word offset, byte val);

		virtual void queue_key_press(byte key);
		virtual word dequeue_serial_output(void);

		virtual const char *change_disk(uint8_t drive, const char *filename);
		virtual const uint8_t *screen_buffer(void);

		virtual bool did_screen_change(void);
	};

/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const computer &simulator)
	{
	qword queue_size = 0;

	operator<<(into, (mc6809 &)simulator);

	queue_size = simulator.keyboard_input.size();
	into.write((char *)&queue_size, sizeof(queue_size));
	for (const auto value : simulator.keyboard_input)
		into.write((char *)&value, sizeof(value));

	queue_size = simulator.serial_output.size();
	into.write((char *)&queue_size, sizeof(queue_size));
	for (const auto value : simulator.serial_output)
		into.write((char *)&value, sizeof(value));

	operator<<(into, simulator.terminal);
	operator<<(into, simulator.hard_drive);

	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, computer &simulator)
	{
	qword queue_size = 0;

	operator>>(from, (mc6809 &)simulator);

	from.read((char *)&queue_size, sizeof(queue_size));
	for (qword count = 0; count < queue_size; count++)
		{
		decltype(simulator.keyboard_input)::value_type value;
		from.read((char *)&value, sizeof(value));
		}

	from.read((char *)&queue_size, sizeof(queue_size));
	for (qword count = 0; count < queue_size; count++)
		{
		decltype(simulator.serial_output)::value_type value;
		from.read((char *)&value, sizeof(value));
		}

	operator>>(from, simulator.terminal);
	operator>>(from, simulator.hard_drive);

	return from;
	}

