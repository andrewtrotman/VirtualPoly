/*
	COMPUTER_POLY_1.CPP
	-------------------
	Copyright (c) 2025 Andrew Trotman
*/
#include <unistd.h>

#include <fstream>
#include <filesystem>

#include "ide.h"
#include "mc6840.h"
#include "keycodes.h"
#include "graphics.h"
#include "ROM_poly.h"
#include "computer_poly_1.h"

extern long mc6854_channel_logging;				// remove this and everything to do with it.

/*
	COMPUTER_POLY_1::COMPUTER_POLY_1()
	----------------------------------
*/
computer_poly_1::computer_poly_1() :
	prot(true),
	dat_bank(1),
	screen_changed(false),
	text_page_1(bios + 0xE800),
	text_page_3(bios + 0xEC00),
	drive_select(0),
	acia1(acia1_input, acia1_output)
	{
	memset(bios, 0, sizeof(bios));
	/*
		Programming in 'C' will be a lot easier with curly brackets and square brackets, so we hack the BIOS
		to allow these characters.  This is achieved by changing the key translation table so that these characters
		have unique translations.  It cannot be done by using the actual key value because those are taken by
		the screen editing keys (characer insert, line delete, etc).

		On ROM 3.4, this means changing positions 2, 3, 4, 5, 7, 12, 16, and 20 in the key translation table.
	*/
#ifndef NEVER
	/*
		Standard Networked Poly 2 (usually in a Poly-1 case, but with numeric keypad).
	*/

	/*
		Load the BIOS
	*/
	memcpy(bios + 0xF000, ROM_poly_BIOS_34, 0x1000);					// BIOS: MAGENTA "830519" has networking

	/*
		Set up the keyboard
	*/
	computer_poly_1::load_and_patch_keyboard(2, 0xFA8F);

#else
	/*
		Standalone Poly with local disk drives.
	*/

	/*
		Load the BIOS
	*/
	memcpy(bios + 0xF000, ROM_poly_BIOS_34_local_disk, 0x1000);		// BIOS: RED "831122 WP" has local disk

	/*
		Set up the keyboard
	*/
	computer_poly_1::load_and_patch_keyboard(1, 0xFAA8);

	/*
		Load a disk into the disk drive.
	*/
	ide::move_disk_to_user_space("polysys5.dsk");
	change_disk(0, ide::get_local_filename("polysys5.dsk").string().c_str());

	ide::move_disk_to_user_space("user.dsk");
	change_disk(1, ide::get_local_filename("user.dsk").string().c_str());

#endif

	/*
		Load the BASIC interpreter into the ROM address space.
	*/
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
	/*
		Nothing
	*/
	}

/*
	COMPUTER_POLY_1::LOAD_AND_PATCH_KEYBOARD()
	------------------------------------------
*/
void computer_poly_1::load_and_patch_keyboard(int poly_version, size_t translation_table_base)
	{
	/*
		Set up the keyboard
	*/
	poly_set_keyboard_scan_codes(poly_version);			// emulate the Poly-1 keyboard

	/*
		Patch the keyboard translation table to allow curly and square brackets
	*/
	bios[translation_table_base + key_fake_open_curly * 2] = '{';
	bios[translation_table_base + key_fake_open_curly * 2 + 1] = 28;
	bios[translation_table_base + key_fake_close_curly * 2] = '}';
	bios[translation_table_base + key_fake_close_curly * 2 + 1] = 28;
	bios[translation_table_base + key_fake_open_square * 2] = '[';
	bios[translation_table_base + key_fake_open_square * 2 + 1] = 28;
	bios[translation_table_base + key_fake_close_square * 2] = ']';
	bios[translation_table_base + key_fake_close_square * 2 + 1] = 28;

	bios[translation_table_base + key_fake_close_open_single_quote * 2] = '`';
	bios[translation_table_base + key_fake_close_open_single_quote * 2 + 1] = 28;
	bios[translation_table_base + key_fake_tilde * 2] = '~';
	bios[translation_table_base + key_fake_tilde * 2 + 1] = 28;
	bios[translation_table_base + key_fake_backslash * 2] = '\\';
	bios[translation_table_base + key_fake_backslash * 2 + 1] = 28;
	}

/*
	COMPUTER_POLY_1::RESET()
	------------------------
*/
void computer_poly_1::reset(void)
	{
	mc6809::reset();
	timer.reset();
	prot = true;
	screen_changed = false;
	dat_bank = 1;
	drive_select = 0;
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
		bool leave_prot_now = (prot && leave_prot);

		/*
			Execute the next instruction
		*/
		execute();

		if (leave_prot_now)
			{
			leave_prot = false;
			prot = false;
			}

		long long cycles_spent = this->cycles - cycles;
		for (long long current = 0; current < cycles_spent; current++)
			timer.step();

		/*
			If there's an IRQ or FIRQ pending them process them (push registers and load PC)
			Note that FIRQ takes precidence over IRQ, and might immediately disable IRQ so we
			check FIRQ first, then only check IRQ if there was no FIRQ.  The next instruction will
			happen then IRQ will be checked (and might not happen).
		*/
		if (pia2.is_signaling_irq() || timer.is_signaling_irq() || network.is_signaling_irq())
			do_irq();

		if (network.is_signaling_irq())
			{
			puts("***POLY IRQ");
			}
		/*
			I don't think anything is on FIRQ on the Poly!
		*/
//				do_firq();
		}
	/*
		Check the keyboard buffer to see if there's anything in it - and if so then process it.
	*/
	if (!pia2.is_signaling_irq() && keyboard_input.size() != 0)			// if the pia buffer is empty we can shove the next key into the buffer
		{
		byte head = keyboard_input.front();
		keyboard_input.pop_front();
		pia2.arrived_b(head, 1 << 7, 0);				// the key has been pressed
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
void computer_poly_1::render(uint32_t *screen_buffer, bool flash_state)
	{
	/*
		Background colours are half-intensity!
	*/
	static uint32_t background_colour_table[] =
		{
		0x00000000,
		0x00000080,
		0x00008000,
		0x00008080,
		0x00800000,
		0x00800080,
		0x00808000,
		0x00808080
		};

	uint32_t display_context = pia1.in_b() << 8 | pia1.in_a();

	/*
		Background (half-intensity)
	*/
	for (size_t pos = 0; pos < 480 * 240; pos++)
		screen_buffer[pos] = background_colour_table[pia1.in_b() >> 4 & 0x07];

	/*
		Graphics screen 4 (not screen 5)
	*/
	if ((display_context & 0x0210) == 0x0200)			// screen 4 (no 480 mode (i.e. "regular" screen 4)
		graphics::render_page(screen_buffer, memory + 0x8000, (pia1.in_b() & 0x0C) >> 2);

	/*
		Text screen 3
	*/
	if (display_context & 0x0100)			// screen 3
		text_page_3.paint_text_page(screen_buffer, flash_state);

	/*
		Graphics screen 5 (2 and 4 interlaces)
	*/
	if ((display_context & 0x0030) == 0x0030)			// screen 5
		graphics::render_combined_pages(screen_buffer, memory + 0x4000, memory + 0x8000);

	/*
		Graphics screen 2 (not screen 5)
	*/
	if ((display_context & 0x0030) == 0x0020)			// screen 2
		graphics::render_page(screen_buffer, memory + 0x4000, (pia1.in_a() & 0x06) >> 1);

	/*
		Graphics screen 2 and 4 colour mixed
	*/
	if ((display_context & 0x0270) == 0x0260)			// screens 2 and 4 colour mixed (the routine below only draws mixed pixels)
		graphics::render_mixed_pages(screen_buffer, memory + 0x4000, memory + 0x8000);

	/*
		Text screen 1
	*/
	if (display_context & 0x0008)			// screen 1
		text_page_1.paint_text_page(screen_buffer, flash_state);
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
	long error_code;

	fdc[drive].mount_disk(filename, &error_code);
	return filename;
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
	0000-DFFF RAM user-mode decoded RAM
	E000-E003 PIA   (MC6821)			Video Controller

	E004-E005 ACIA  (MC6850)			Serial Port
	E006      Baud Rate Register		Baud Rate Controller
	E00C-E00F PIA   (MC6821)			Keyboard
	E014      DISK  (WD1771)         Disc Change Indicator
	E018-E01B DISK  (WD1771)         Floppy Disk Drive Controller
	E020-E027 PTM   (MC6840)			Real Time Clock
	E030-E036 ADLC  (MC6854)         Networking
	E040      PROT                   Prot switch
	E050-E05F DAT                    Dynamic Address Translator (page table)
	E060      MMU                    Memory Map 1 select
	E070      MMU                    Memory Map 2 select
	E800-EBBF TEXT (SAA5050)         Teletext 1 screen
	EBC0-EBFF RAM                    System data
	EC00-EFBF TEXT (SAA5050)         Teletext 2 screen
	EFC0-EFFF RAM                     System data
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
				Screen controller
				MC6821 PIA (Parallel Controller) at E000-E003
			*/
			case 0xE000:
			case 0xE001:
			case 0xE002:
			case 0xE003:
				answer = pia1.read(raw_address - 0xE000);
				break;

			/*
				Serial port (optional)
				Used as a cassette interface on Poly 1 (with early BASIC versions)
				MC6850 ACIA (Serial Controller) at E004-E00
			*/
			case 0xE004:
			case 0xE005:
				answer = acia1.read(raw_address - 0xE004);
				break;

			/*
				Baud rate register at 0xE006
			*/
			case 0xE006:
				answer = baud_rate_register;
				break;

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
				Disc Change Indicator
				bit 1 (000000x0) indicated whether the current disk drive door has been opened since the drive was last deselected.
				The bit is cleared when a new drive is selected
			*/
			case 0xE014:
				answer = fdc[drive_select & 0x03].get_door_opened() ? 0x02 : 0x00;
				break;

			/*
				FDC
				WD1771 FDC (Floppy Drive Controller) at 0xE018-E01B
			*/
			case 0xE018:
			case 0xE019:
			case 0xE01A:
			case 0xE01B:
				answer = fdc[drive_select & 0x03].read(raw_address - 0xE018);
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
				answer = timer.read(raw_address - 0xE020);
				break;

			/*
				Network
				MC6854 ADLC (network) at E030-E036
			*/
			case 0xE030:
			case 0xE031:
			case 0xE032:
			case 0xE033:
			case 0xE034:
			case 0xE035:
			case 0xE036:
				mc6854_channel_logging = true;
				answer = network.read((raw_address - 0xE030) / 2);
				mc6854_channel_logging = false;
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
	0000-DFFF RAM user-mode decoded RAM (see below)
	E000-E003 PIA   (MC6821)			Video Controller
	E00C-E00F PIA   (MC6821)			Keyboard
	E014      DISK  (WD1771)         Drive and Side Select
	E018-E01B DISK  (WD1771)         Floppy Disk Drive Controller
	E020-E027 PTM   (MC6840)			Real Time Clock
	E030-E036 ADLC  (MC6854)         Networking
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
				Screen controller
				MC6821 PIA (Parallel Controller) at E000-E003
			*/
			case 0xE000:
			case 0xE001:
			case 0xE002:
			case 0xE003:
				pia1.write(raw_address - 0xE000, value);
				break;

			/*
				Serial port (optional)
				Used as a cassette interface on Poly 1 (with early BASIC versions)
				MC6850 ACIA (Serial Controller) at E004-E00
			*/
			case 0xE004:
			case 0xE005:
				acia1.write(raw_address - 0xE004, value);
				break;

			/*
				Baud rate register at 0xE006
			*/
			case 0xE006:
				baud_rate_register = value;
				break;

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
				Drive and Side Select
				lower 2 bits are the drive; bit 6 is the side select (0x0000xx)
			*/
			case 0xE014:
				if (value != drive_select)
					{
					/*
						Only clear the status on deselect
					*/
					fdc[drive_select & 0x03].clear_door_opened();
					drive_select = value;
					}
				break;

			/*
				FDC
				WD1771 FDC (Floppy Drive Controller) at 0xE018-E01B
			*/
			case 0xE018:
			case 0xE019:
			case 0xE01A:
			case 0xE01B:
				fdc[drive_select & 0x03].write(raw_address - 0xE018, value);
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
				timer.write(raw_address - 0xE020, value);
				break;

			/*
				Network
				MC6854 ADLC (network) at E030-E036
			*/
			case 0xE030:
			case 0xE031:
			case 0xE032:
			case 0xE033:
			case 0xE034:
			case 0xE035:
			case 0xE036:
				mc6854_channel_logging = true;
				network.write((raw_address - 0xE030) / 2, value);
				mc6854_channel_logging = false;
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
	auto changes = screen_changed;
	screen_changed = false;
	return changes;
	}

/*
	COMPUTER_POLY_1::QUEUE_KEY_PRESS()
	----------------------------------
*/
void computer_poly_1::queue_key_press(byte key)
	{
	keyboard_input.push_back(key);
	}

/*
	COMPUTER_POLY_1::QUEUE_KEY_RELEASE()
	------------------------------------
*/
void computer_poly_1::queue_key_release(byte key)
	{
//	pia2.arrived_b(key, 0, 0);						// the key has been released
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
