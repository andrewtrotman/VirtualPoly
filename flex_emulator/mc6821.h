/*
	MC6821.H
	--------
*/
#pragma once

#include <stdio.h>
#include <stdint.h>

/*
	class MC6821
	------------
	Motorola 6821 PIA (Parallel Port)
	Default behaviour is to have a single byte buffer attached to the port that can be written to and read from
*/
class mc6821
	{
	protected:
		uint8_t cra, dda, byte_a;
		uint8_t crb, ddb, byte_b;

	protected:
		virtual void irqa1(void)		{ cra |= 0x80; }
		virtual void irqa2(void)		{ cra |= 0x40; }
		virtual void d_irqa(void)		{ cra &= 0x3F; }

		virtual void irqb1(void)		{ crb |= 0x80; }
		virtual void irqb2(void)		{ crb |= 0x40; }
		virtual void d_irqb(void)		{ crb &= 0x3F; }

	public:
		mc6821();
		virtual ~mc6821() {}

		uint8_t read(uint16_t address);
		void write(uint16_t address, uint8_t value);

		virtual uint8_t in_a(void) 		{ return byte_a; }
		virtual void out_a(uint8_t value)
			{
			byte_a = value;
//			printf("%c", value);
			}
		virtual uint8_t in_b(void)			{ return byte_b; }
		virtual void out_b(uint8_t value)
			{
			byte_b = value;
//			printf("%c", value);
			}
		virtual void arrived_a(uint8_t value, uint8_t ca1, uint8_t ca2);
		virtual void arrived_b(uint8_t value, uint8_t cb1, uint8_t cb2);
	} ;
