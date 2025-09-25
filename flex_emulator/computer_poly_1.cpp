/*
	COMPUTER_POLY_1.CPP
	-------------------
	Copyright (c) 2025 Andrew Trotman
*/
#include <fstream>
#include <filesystem>

#include "ROM_poly.h"
#include "mc6840_with_irq.h"
#include "computer_poly_1.h"

/*
	COMPUTER_POLY_1::COMPUTER_POLY_1()
	----------------------------------
*/
computer_poly_1::computer_poly_1() :
	prot(true),
	cycles_before_leaving_prot(-1),
	dat_bank(1),
	screen_changed(false),
	text_page_1(bios + 0xE800),
	text_page_3(bios + 0xEC00),
	timer(new mc6840_with_irq(this))
	{
	memset(bios, 0, sizeof(bios));
	memcpy(bios + 0xF000, ROM_poly_BIOS_34, 0x1000);

	memcpy(memory + 0xC000, ROM_poly_BASIC_34_1, 0x1000);
	memcpy(memory + 0xD000, ROM_poly_BASIC_34_2, 0x1000);
	memcpy(memory + 0xE000, ROM_poly_BASIC_34_3, 0x1000);
	memcpy(memory + 0xF000, ROM_poly_BASIC_34_4, 0x1000);
	}

/*
	COMPUTER_POLY_1::~COMPUTER_POLY_1()
	-----------------------------------
*/
computer_poly_1::~computer_poly_1()
	{
	delete timer;
	}

/*
	COMPUTER_POLY_1::RESET()
	------------------------
*/
void computer_poly_1::reset(void)
	{
	mc6809::reset();
	timer->reset();
	prot = true;
	cycles_before_leaving_prot = -1;
	screen_changed = false;
	dat_bank = 1;
	}

/*
	COMPUTER_POLY_1::STEP()
	-----------------------
*/
void computer_poly_1::step(uint64_t times)
	{
	for (uint64_t count = 0; count < times; count++)
		{
		/*
			Setup - are we about to leave protected mode (etc.).
		*/
		long long cycles = this->cycles;
		start_of_instruction = pc;
		bool leave_prot_now = (prot && cycles_before_leaving_prot > 0);

		/*
			Execute the next instruction
		*/
		execute();

		long long cycles_spent = this->cycles - cycles;
		if (leave_prot_now && (cycles_before_leaving_prot -= cycles_spent) <= 0)
			{
			cycles_before_leaving_prot = -1;
			prot = false;
			}

//		for (long long current = 0; current < cycles_spent; current++)
//			timer->step();

//printf("%lld ", cycles_spent);
		/*
			If there's an IRQ or FIRQ pending them process them (push registers and load PC)
			Note that FIRQ takes precidence over IRQ, and might immediately disable IRQ so we
			check FIRQ first, then only check IRQ if there was no FIRQ.  The next instruction will
			happen then IRQ will be checked (and might not happen).
		*/
		if (firqpend)
			do_firq();
		else if (irqpend)
			do_irq();
		}
	}

/*
	COMPUTER_POLY_1::GET_SERIALISED_FILENAME()
	------------------------------------------
*/
std::string computer_poly_1::get_serialised_filename(void)
	{
	return (std::filesystem::path(getenv("HOME")) / std::filesystem::path("Documents/emulator.state")).string();
	}

/*
	COMPUTER_POLY_1::SCREEN_BUFFER()
	--------------------------------
*/
const uint8_t *computer_poly_1::screen_buffer(void)
	{
	return bios + 0xE800;
	}

/*
	COMPUTER_POLY_1::RENDER()
	-------------------------
*/
void computer_poly_1::render(uint32_t *screen_buffer)
	{
	text_page_1.paint_text_page(screen_buffer, false);
	}

/*
	COMPUTER_POLY_1::SERIALISE()
	----------------------------
*/
void computer_poly_1::serialise(void)
	{
	auto file = std::ofstream(get_serialised_filename());
	file << *this;
	file.close();
	}

/*
	COMPUTER_POLY_1::DESERIALISE()
	------------------------------
*/
void computer_poly_1::deserialise(void)
	{
	auto file = std::ifstream(get_serialised_filename());
	file >> *this;
	file.close();
	}

/*
	COMPUTER_POLY_1::CHANGE_DISK()
	------------------------------
*/
const char *computer_poly_1::change_disk(uint8_t drive, const char *filename)
	{
	return "No disk loaded";
	}

/*
	COMPUTER_POLY_1::RAW_TO_PHYSICAL()
	----------------------------------
	Translate a logical address into a physical address
	dat_bank selects which of the 2 DAT banks we use
	the top 3 bits of the address select the DAT register
	the dat register forms the top 8 address bits
	the bottom 13 bits of the address are the offset
	remember... the DAT is in 1's compliment

	[DAT][ADDRESS] = DDDD DDDA AAAA AAAA AAAA
	where DAT = [B][ADDRESS] B AAA

	Windows Emulator version:
		return (((~dat[(dat_bank << 3) | (address >> 13)]) & 0x0F) << 13) | (address & 0x1FFF);

	Note that this emulator expands the address space to 2MB but the Poly only had 128KB
*/
qword computer_poly_1::raw_to_physical(word raw_address)
	{
	qword dat_register = (raw_address >> 13) + 8 * (dat_bank - 1);
	qword top_bits = ~bios[0xE050 + dat_register] & 0xFF;
	qword physical_address =  (raw_address & 0x1FFF) | (top_bits << 13);

	return physical_address;
	}

/*
	COMPUTER_POLY_1::READ()
	-----------------------
	E00C-E00F PIA   (MC6821)			Keyboard
	E040      PROT switch
	E050-E05F Dynamic Address Translator (page table)
	E060      Memory Map 1 select
	E070      Memory Map 2 select
	F000-FFFF ROM
*/
byte computer_poly_1::read(word raw_address)
	{
	byte answer;
	if (!prot)
		answer = memory[raw_to_physical(raw_address)];				// User mode addressing
	else
		{
		switch (raw_address)
			{
			/*
				Keyboard controler
				MC6821 PIA (Parallel Controller) at E00C-E00F
			*/
			case 0xE00C:
			case 0xE00D:
			case 0xE00E:
			case 0xE00F:
				answer = pia2.read(raw_address - 0xE00C);
				break;

			/*
				Timer
				MC6840 Programmable Timer (Clock) at E020-E027
			*/
			case 0xE020:
			case 0xE021:
			case 0xE022:
			case 0xE023:
			case 0xE024:
			case 0xE025:
			case 0xE026:
			case 0xE027:
				answer = timer->read(raw_address - 0xE020);
				break;

			/*
				Prot switch at 0xE040
			*/
			case 0xE040:
				answer = 0;
				break;

			/*
				DAT Dynamic Address Translation at 0xE050-E05F
			*/
			case 0xE050:
			case 0xE051:
			case 0xE052:
			case 0xE053:
			case 0xE054:
			case 0xE055:
			case 0xE056:
			case 0xE057:
			case 0xE058:
			case 0xE059:
			case 0xE05A:
			case 0xE05B:
			case 0xE05C:
			case 0xE05D:
			case 0xE05E:
			case 0xE05F:
				answer = bios[raw_address];		 // this is the page lookup table (in 1s compliment)
				break;

			/*
				Memory Map 1 select at 0xE060
			*/
			case 0xE060:
				answer = 0;
				break;

			/*
				Memory Map 2 select at 0xE70
			*/
			case 0xE070:
				answer = 0;
				break;

			/*
				Protected mode memory (BIOS, text screen, etc)
			*/
			default:
				if (raw_address < 0xE000)
					answer = memory[raw_to_physical(raw_address)];				// User mode addressing
				else if (raw_address < 0xE800)
					answer = 0; 							// there's nothing here in the memory map
				else
					answer = bios[raw_address];
				break;
			}
		}

	return answer;
	}

/*
	COMPUTER_POLY_1::WRITE()
	------------------------
	E040      PROT switch
	E050-E05F Dynamic Address Translator (page table)
	E060      Memory Map 1 select
	E070      Memory Map 2 select
	E800-EBBF Teletext 1 screen
	EBC0-EBFF RAM System data
	EC00-EFBF Teletext 2 screen
	EFC0-EFFF RAM System data
	F000-FFFF ROM
*/
void computer_poly_1::write(word raw_address, byte value)
	{
	if (!prot)
		{
		unsigned long physical_address;

		/*
			User mode address translation
		*/
		physical_address = raw_to_physical(raw_address);
		if (physical_address < 0xC000 || physical_address >= 0x10000)		// else we're in the BASIC ROM
			memory[physical_address] = value;
		}
	else
		{
		switch (raw_address)
			{
			/*
				Keyboard controler
				MC6821 PIA (Parallel Controller) at E00C-E00F
			*/
			case 0xE00C:
			case 0xE00D:
			case 0xE00E:
			case 0xE00F:
				pia2.write(raw_address - 0xE00C, value);
				break;

			/*
				Timer
				MC6840 Programmable Timer (Clock) at E020-E027
			*/
			case 0xE020:
			case 0xE021:
			case 0xE022:
			case 0xE023:
			case 0xE024:
			case 0xE025:
			case 0xE026:
			case 0xE027:
				timer->write(raw_address - 0xE020, value);
				break;

			/*
				Prot switch
				We leave prot at the end of the next instruction.
				This is so that we can write to E040 in one instruction and then RTI in the next (RTI takes 2 clock cycles).
			*/
			case 0xE040:
				cycles_before_leaving_prot = 2;
				break;

			/*
				DAT Dynamic Address Translation at 0xE050-E05F
			*/
			case 0xE050:
			case 0xE051:
			case 0xE052:
			case 0xE053:
			case 0xE054:
			case 0xE055:
			case 0xE056:
			case 0xE057:
			case 0xE058:
			case 0xE059:
			case 0xE05A:
			case 0xE05B:
			case 0xE05C:
			case 0xE05D:
			case 0xE05E:
			case 0xE05F:
				bios[raw_address] = value;
				break;

			/*
				Memory Map 1 select at 0xE060
			*/
			case 0xE060:
				dat_bank = 1;
				break;

			/*
				Memory Map 2 select at 0xE70
			*/
			case 0xE070:
				dat_bank = 2;
				break;

			/*
				Computer Memory (including text screen and system scratch RAM)
			*/
			default:
				if (raw_address < 0xE000)
					{
					/*
						User mode addressing
						This is done with a recursive call so that we can catch any writes to the screen and update the flags
						It also stops write to the BASIC ROMs
					*/
					prot = false;
					write(raw_address, value);
					prot = true;						// because we're already in PROT
					}
				else if (raw_address < 0xE800)
					{ /* there's nothing here in the memory map */ }
				else if (raw_address < 0xF000)
					{
					bios[raw_address] = value;
					if (raw_address >= 0xE800 && raw_address < 0xEBC0)
							screen_changed = true;				// write to text page 1
					else if (raw_address >= 0xEC00 && raw_address < 0xEFC0)
							screen_changed = true;				// write to text page 1
					}
				else
					{ /* we're in the BIOS so can't write */ }
			}
		}
	}

/*
	COMPUTER_POLY_1::DID_SCREEN_CHANGE()
	------------------------------------
*/
bool computer_poly_1::did_screen_change(void)
	{
	return screen_changed;
	}

/*
	COMPUTER_POLY_1::QUEUE_KEY_PRESS()
	----------------------------------
*/
void computer_poly_1::queue_key_press(byte key)
	{
	pia2.arrived_b(key, 1 << 7, 0);				// the key has been pressed
	queue_irq();
	}

/*
	COMPUTER_POLY_1::QUEUE_KEY_RELEASE()
	------------------------------------
*/
void computer_poly_1::queue_key_release(byte key)
	{
//	pia2.arrived_b(key, 0, 0);						// the key has been released
//	queue_irq();
	}

/*
	COMPUTER_POLY_1::SWI()
	----------------------
*/
void computer_poly_1::swi(void)
	{
	prot = true;
	mc6809::swi();
	}

/*
	COMPUTER_POLY_1::SWI2()
	-----------------------
*/
void computer_poly_1::swi2(void)
	{
	prot = true;
	mc6809::swi2();
	}

/*
	COMPUTER_POLY_1::SWI3()
	-----------------------
*/
void computer_poly_1::swi3(void)
	{
	prot = true;
	mc6809::swi3();
	}

/*
	COMPUTER_POLY_1::DO_NMI()
	-------------------------
*/
void computer_poly_1::do_nmi(void)
	{
	prot = true;
	mc6809::do_nmi();
	}

/*
	COMPUTER_POLY_1::DO_FIRQ()
	--------------------------
*/
void computer_poly_1::do_firq(void)
	{
	prot = true;
	mc6809::do_firq();
	}

/*
	COMPUTER_POLY_1::DO_IRQ()
	-------------------------
*/
void computer_poly_1::do_irq(void)
	{
	prot = true;
	mc6809::do_irq();
	}
