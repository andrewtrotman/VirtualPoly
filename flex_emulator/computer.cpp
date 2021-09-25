/*
	COMPUTER.CPP
	------------
	Copyright (c) 2021 Andrew Trotman
*/
#include <fstream>
#include <filesystem>

#include "ROM_FLEX.h"
#include "computer.h"

/*
	COMPUTER::COMPUTER()
	--------------------
*/
computer::computer() :
	terminal(keyboard_input, serial_output),
	prot(true),
	leave_prot(false),
	screen_changed(false)
	{
	memset(bios, 0, sizeof(bios));
	memcpy(bios + 0xF000, ROM_FLEX, 0x1000);
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
	COMPUTER::RESET()
	-----------------
*/
void computer::reset(void)
	{
	mc6809::reset();
	prot = true;
	leave_prot = false;
	screen_changed = false;
	}

/*
	COMPUTER::STEP()
	----------------
*/
void computer::step(uint64_t times)
	{
	for (uint64_t count = 0; count < times; count++)
		{
		start_of_instruction = pc;
		bool leave_prot_now = leave_prot;
		execute();
		if (leave_prot_now)
			prot = leave_prot = false;
		}
	}

/*
	COMPUTER::GET_SERIALISED_FILENAME()
	-----------------------------------
*/
std::string computer::get_serialised_filename(void)
	{
	return (std::filesystem::path(getenv("HOME")) / std::filesystem::path("Documents/emulator.state")).string();
	}

/*
	COMPUTER::SCREEN_BUFFER()
	-------------------------
*/
const uint8_t *computer::screen_buffer(void)
	{
	return nullptr;
	}

/*
	COMPUTER::SERIALISE()
	---------------------
*/
void computer::serialise(void)
	{
	hard_drive.disk_save(0);
	hard_drive.disk_save(1);
	auto file = std::ofstream(get_serialised_filename());
	file << *this;
	file.close();
	}

/*
	COMPUTER::DESERIALISE()
	-----------------------
*/
void computer::deserialise(void)
	{
	auto file = std::ifstream(get_serialised_filename());
	file >> *this;
	file.close();
	}

/*
	COMPUTER::CHANGE_DISK()
	-----------------------
*/
const char *computer::change_disk(uint8_t drive, const char *filename)
	{
	hard_drive.disk_save(drive);
	hard_drive.disk_load(drive, filename);
	disk_name = hard_drive.flex_diskname(drive);
	return disk_name.c_str();
	}

/*
	COMPUTER::READ()
	----------------
	E004-E005 ACIA  (MC6850)
	E010-E017 IDE hard drive controller
	F000-FFFF ROM
*/
byte computer::read(word raw_address)
	{
	byte answer;
	uint32_t address = raw_address;

	if (prot && raw_address >= 0xE000)
		{
		switch (raw_address)
			{
			/*
				Terminal RS232 Port
			*/
			case 0xE004:
			case 0xE005:
				answer = terminal.read(raw_address - 0xE004);
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
				answer = hard_drive.read(raw_address - 0xE010);
				break;

			/*
				Printer on 6821 PIA
			*/
			case 0xE01C:
			case 0xE01D:
			case 0xE01E:
			case 0xE01F:
				answer = printer.read(raw_address - 0xE01C);
				break;

			/*
				Prot switch at 0xE040
			*/
			case 0xE040:
				answer = 0;
				break;

			/*
				Protected mode memory (BIOS, text screen, etc)
			*/
			default:
				answer = bios[raw_address];
			}
		}
	else
		answer = memory[address];

	return answer;
	}

/*
	COMPUTER::WRITE()
	-----------------
	E004-E005 ACIA  (MC6850)
	E010-E017 IDE hard drive controller
	F000-FFFF ROM
*/
void computer::write(word raw_address, byte value)
	{
	uint32_t address = raw_address;

	if (prot && raw_address >= 0xE000)
		{
		switch (raw_address)
			{
			/*
				Terminal RS232 Port
			*/
			case 0xE004:
			case 0xE005:
				terminal.write(raw_address - 0xE004, value);
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
				hard_drive.write(raw_address - 0xE010, value);
				break;

			/*
				Printer on 6821 PIA
			*/
			case 0xE01C:
			case 0xE01D:
			case 0xE01E:
			case 0xE01F:
				printer.write(raw_address - 0xE01C, value);
				break;

			/*
				Prot switch
				We leave prot at the end of the next instruction.
				This is so that we can write to E040 in one instruction and then RTI in the next (RTI takes 2 clock cycles).
			*/
			case 0xE040:
				leave_prot = true;
				break;

			/*
				Computer Memory
			*/
			default:
				if (raw_address < 0xF000)
					bios[raw_address] = value;
			}
		}
	else
		memory[address] = value;
	}

/*
	COMPUTER::DID_SCREEN_CHANGE()
	-----------------------------
*/
bool computer::did_screen_change(void)
	{
	return screen_changed;
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
