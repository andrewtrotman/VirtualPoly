/*
	MC6821.CPP
	----------
*/
#include "mc6821.h"

/*
	MC6821::MC6821()
	----------------
*/
mc6821::mc6821()
	{
	byte_a = byte_b = 0;
	cra = crb = dda = ddb = 0;
	}

/*
	MC6821::READ()
	--------------
*/
uint8_t mc6821::read(uint16_t address)
	{
	uint8_t answer;

	switch (address)
		{
		case 0x0000:
			if ((cra & 0x04) == 0)
				answer = dda;
			else
				{
				d_irqa();
				answer = in_a(); 		// should this be: answer = in_a() & ~dda;
				}
			return answer;

		case 0x0001:
			return cra | 0X80;

		case 0x0002:
			if ((crb & 0x04) == 0)
				answer = ddb;
			else
				{
				d_irqb();
				answer = in_b();		// should this be: answer = in_b() & ~ddb;
				}
			return answer;

		case 0x0003:
			return crb | 0X80;;

		default:
			return 0;
		}
	}

/*
	MC6821::WRITE()
	---------------
*/
void mc6821::write(uint16_t address, uint8_t value)
	{
	switch (address)
		{
		case 0x0000:
			if ((cra & 0x04) == 0)			// Data Direction Access Control
				dda = value;
			else
				out_a(value & dda);
			break;

		case 0x0001:
			cra = value & 0x3F;		// we can only write to bits 0..5
			break;

		case 0x0002:
			if ((crb & 0x04) == 0)			// Data Direction Access Control
				ddb = value;
			else
				out_b(value & ddb);
			break;

		case 0x0003:
			crb = value & 0x3F;		// we can only write to bits 0..5
			break;
		}
	}

/*
	MC6821::ARRIVED_A()
	-------------------
	Call this if data arrives at port A
*/
void mc6821::arrived_a(uint8_t value, uint8_t ca1, uint8_t ca2)
	{
	byte_a = value;
	if ((ca1 != 0) && (cra & 0x01))
		irqa1();
	if ((ca2 != 0) && (cra & 0x20) && (cra & 0x08))
		irqa2();
	}

/*
	MC6821::ARRIVED_B()
	-------------------
	Call this if data arrives at port B
*/
void mc6821::arrived_b(uint8_t value, uint8_t cb1, uint8_t cb2)
	{
	byte_b = value;
	if ((cb1 != 0) && (crb & 0x01))
		irqb1();
	if ((cb2 != 0) && (crb & 0x20) && (crb & 0x08))
		irqb2();
	}
