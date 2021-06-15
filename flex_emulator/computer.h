/*
	COMPUTER.H
	----------
*/
#include "ide.h"
#include "mc6809.h"
#include "mc6850.h"
#include "saa5050_font.h"

/*
	CLASS COMPUTER
	--------------
*/
class computer : public mc6809
	{
	private:
		std::deque<byte> keyboard_input;
		std::deque<byte> serial_output;
		mc6850 terminal;
		ide hard_drive;

	public:
		computer();
		virtual ~computer();

		virtual byte read(word offset);
		virtual void write(word offset, byte val);

		virtual void queue_key_press(byte key);
		virtual word dequeue_serial_output(void);
	};
