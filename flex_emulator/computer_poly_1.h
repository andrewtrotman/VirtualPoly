/*
	COMPUTER_POLY_1.H
	-----------------
	Copyright (c) 2025 Andrew Trotman
*/
#pragma once

#include "mc6840.h"
#include "saa5050.h"
#include "computer.h"

/*
	CLASS COMPUTER_POLY_1
	---------------------
*/
class computer_poly_1 : public computer
	{
	friend std::ostream &operator<<(std::ostream &into, const computer_poly_1 &simulator);
	friend std::istream &operator>>(std::istream &from, computer_poly_1 &simulator);

	protected:
		bool prot;						// protected (BIOS) mode?
		uint8_t dat_bank;				// which of the two DAT tables to use
		std::string disk_name;
		bool screen_changed;
		uint8_t bios[0x10000];		// the BIOS is in a seperate space from the RAM
		saa5050 text_page_1;
		saa5050 text_page_3;
		mc6821 pia1;					// Poly video control PIA, including background colours, which screens are displayed, etc.
		mc6821 pia2;					// Poly keyboard interface is an ASCII-like keyboard attached to an mc6821
		mc6840 timer;					// The Poly timer, used for sound (and other stuff too)

	private:
		std::string get_serialised_filename();

	public:
		computer_poly_1();
		virtual ~computer_poly_1();

		virtual void reset(void);
		virtual void step(uint64_t times);

		virtual void do_nmi(void);
		virtual void do_firq(void);
		virtual void do_irq(void);
		virtual void swi(void);
		virtual void swi2(void);
		virtual void swi3(void);

		virtual void serialise(void);
		virtual void deserialise(void);

		virtual qword raw_to_physical(word raw_address);

		virtual byte read(word offset);
		virtual void write(word offset, byte val);

		virtual void queue_key_press(byte key);
		virtual void queue_key_release(byte key);

		virtual void timer_irq() {}
		virtual void timer_d_irq() {}

		virtual const char *change_disk(uint8_t drive, const char *filename);
		virtual const uint8_t *screen_buffer(void);

		virtual bool did_screen_change(void);
		virtual void render(uint32_t *screen_buffer, bool flash_state);
	};

/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const computer_poly_1 &simulator)
	{
	qword queue_size = 0;

	operator<<(into, (mc6809 &)simulator);
	into.write((char *)&simulator.bios[0], sizeof(simulator.bios));

	into.write((char *)&simulator.prot, sizeof(simulator.prot));
	into.write((char *)&simulator.leave_prot, sizeof(simulator.leave_prot));
	into.write((char *)&simulator.dat_bank, sizeof(simulator.dat_bank));

	queue_size = simulator.keyboard_input.size();
	into.write((char *)&queue_size, sizeof(queue_size));
	for (const auto value : simulator.keyboard_input)
		into.write((char *)&value, sizeof(value));

	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, computer_poly_1 &simulator)
	{
	qword queue_size = 0;

	operator>>(from, (mc6809 &)simulator);
	from.read((char *)&simulator.bios[0], sizeof(simulator.bios));

	from.read((char *)&simulator.prot, sizeof(simulator.prot));
	from.read((char *)&simulator.leave_prot, sizeof(simulator.leave_prot));
	from.read((char *)&simulator.dat_bank, sizeof(simulator.dat_bank));

	from.read((char *)&queue_size, sizeof(queue_size));
	for (qword count = 0; count < queue_size; count++)
		{
		decltype(simulator.keyboard_input)::value_type value;
		from.read((char *)&value, sizeof(value));
		}

	simulator.screen_changed = true;

	return from;
	}

