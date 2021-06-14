/*
	COMPUTER.CPP
	------------
*/
#include "ROM_Pinnated.h"
#include "ROM_TinyBasic.h"
#include "ROM_FLEX.h"
#include "computer.h"

/*
	COMPUTER::COMPUTER()
	--------------------
*/
computer::computer() :
	terminal(keyboard_input, serial_output)
	{
	memcpy(memory + 0xF000, eprom_TinyBasic, 0x1000);

	/* Nothing */
	}

/*
	COMPUTER::~COMPUTER()
	---------------------
*/
computer::~computer()
	{
	/*	Nothing */
	}

/*
	COMPUTER::READ()
	----------------
	E004-E005 ACIA  (MC6850)
	F000-FFFF ROM
*/
byte computer::read(word address)
	{
	byte answer;

	switch (address)
		{
		/*
			Terminal RS232 Port
		*/
		case 0xE004:
		case 0xE005:
			answer = terminal.read(address - 0xE004);
			break;

		/*
			Computer Memory
		*/
		default:
			answer = memory[address];
		}

	return answer;
	}

/*
	COMPUTER::WRITE()
	-----------------
	E004-E005 ACIA  (MC6850)
	F000-FFFF ROM
*/
void computer::write(word address, byte value)
	{
	switch (address)
		{
		/*
			Terminal RS232 Port
		*/
		case 0xE004:
		case 0xE005:
			terminal.write(address - 0xE004, value);
			break;

		/*
			Computer Memory
		*/
		default:
			if (address < 0xE000)
				memory[address] = value;
		}
	}

/*
	COMPUTER::QUEUE_KEY_PRESS()
	---------------------------
*/
void computer::queue_key_press(byte key)
{
keyboard_input.push_back(key);
}

/*
	COMPUTER::DEQUEUE_SERIAL_OUTPUT()
	---------------------------------
*/
word computer::dequeue_serial_output(void)
{
if (serial_output.empty())
	return 0xFFFF;

word answer = serial_output.front();
serial_output.pop_front();

return answer;
}
