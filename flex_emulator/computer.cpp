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
	memcpy(memory + 0xF000, eprom_FLEX, 0x1000);
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
	E010-E017 IDE hard drive controller
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
			IDE hard disk controller
		*/
		case 0xE010:
		case 0xE011:
		case 0xE012:
		case 0xE013:
		case 0xE014:
		case 0xE015:
		case 0xE016:
		case 0xE017:
			answer = hard_drive.read(address - 0xE010);
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
	E010-E017 IDE hard drive controller
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
			IDE hard disk controller
		*/
		case 0xE010:
		case 0xE011:
		case 0xE012:
		case 0xE013:
		case 0xE014:
		case 0xE015:
		case 0xE016:
		case 0xE017:
			hard_drive.write(address - 0xE010, value);
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
