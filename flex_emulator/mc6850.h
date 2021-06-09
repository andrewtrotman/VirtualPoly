/*
	MC6850.H
	--------
*/
#ifndef MC6850_H_
#define MC6850_H_

#include <deque>

#include "typedefs.h"

/*
	class MC6850
	------------
	Motorola 6850 ACIA (Serial Port)
*/
class mc6850
	{
	private:
		byte baud_rate_register;
		std::deque<byte> &input;
		std::deque<byte> &output;

	protected:
		virtual void reset(void);
		virtual byte poll(void);
		virtual byte in(void);
		virtual void out(byte value);

	public:
		mc6850(std::deque<byte> &input, std::deque<byte> &output) :
			baud_rate_register(0),
			input(input),
			output(output)
			{
			reset();
			}

		virtual ~mc6850()
			{
			/* Nothing */
			}

		byte read(word address);
		void write(word address, byte value);
	};

#endif /* MC6850_H_ */
