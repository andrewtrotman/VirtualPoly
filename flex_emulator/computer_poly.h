/*
	COMPUTER_POLY.H
	---------------
	Copyright (c) 2025 Andrew Trotman
*/
#pragma once

#include <deque>

#include "mc6840.h"
#include "mc6854.h"
#include "wd1771.h"
#include "saa5050.h"
#include "computer.h"

/*
	CLASS COMPUTER_POLY
	-------------------
*/
class computer_poly : public computer
	{
	friend std::ostream &operator<<(std::ostream &into, const computer_poly &simulator);
	friend std::istream &operator>>(std::istream &from, computer_poly &simulator);

	protected:
		class key_event
			{
			public:
				byte key;				// the key being actioned
				byte up_down;			// is it being pressed or released?
			public:
				key_event(byte key, byte up_down) :
					key(key),
					up_down(up_down)
					{
					/* Nothing */
					}
			};

	protected:
		bool prot;						// protected (BIOS) mode?
		uint8_t dat_bank;				// which of the two DAT tables to use
		std::string disk_name;
		bool screen_changed;
		uint8_t bios[0x10000];		// the BIOS is in a seperate space from the RAM
		saa5050 text_page_1;
		saa5050 text_page_3;
		mc6821 pia1;					// Poly video control PIA, including background colours, which screens are displayed, etc.
		std::deque<key_event> keyboard_input;
		mc6821 pia2;					// Poly keyboard interface is an ASCII-like keyboard attached to an mc6821
		mc6840 timer;					// The Poly timer, used for sound (and other stuff too)
		wd1771 fdc[4];					// The Poly (and Proteus) support up-to 4 disk drives (this appears to be a FLEX limit)
		unsigned char drive_select;// Which disk drive is currently selected
		std::deque<uint8_t> acia1_input;
		std::deque<uint8_t> acia1_output;
		mc6850 acia1;
		unsigned char baud_rate_register;
		mc6854 network;

	private:
		std::string get_serialised_filename();
		void load_and_patch_keyboard(int poly_version, size_t translation_table_base);

	public:
		computer_poly(int rom_version);
		virtual ~computer_poly();

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

		bool network_irqpend;
		virtual void network_irq(void);
		virtual void network_d_irq(void);
	};

/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const computer_poly &simulator)
	{
	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, computer_poly &simulator)
	{
	return from;
	}

