/*
	PROTEUS.C
	---------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ROM_proteus.h"
#include "proteus.h"
#include "vice_z80/vice_z80_interface.h"

extern long mc6854_channel_logging;				// remove this and everything to do with it.

/*
	PROTEUS::PROTEUS()
	------------------
*/
proteus::proteus() :
	computer(),
	acia1(keyboard_input, serial_output),
	acia2(acia2_in, acia2_out),
	acia3(acia3_in, acia3_out)
	{
	cpu_frequency = 1000000;												// the 6809 is clocked at 1MHz
	load_rom(30982);
	reset();
	}

/*
	PROTEUS::LOAD_ROM()
	-------------------
*/
unsigned char *proteus::load_rom(long version)
{
switch (version)
	{
	case 30982:
		current_rom = ROM_proteus_BIOS_30;
		break;
	case 300986:
		current_rom = ROM_proteus_BIOS_31;
		break;
	default:
		current_rom = ROM_proteus_BIOS_30;
		break;
	}
reset();
return current_rom;
}

/*
	PROTEUS::RESET()
	----------------
*/
void proteus::reset(void)
{
cpu_active = CPU_6809;

memset(z80_eprom_shadow, 0, sizeof(z80_eprom_shadow));
memcpy(memory + 0xF000, current_rom, 0x1000);			// put the ROM back
mc6809::reset();
z80_reset();
drive_select = 0;
}

/*
	PROTEUS::READ()
	---------------
	0000-DFFF RAM
	E000-E003 PIA   (MC6821)
	E004-E005 ACIA  (MC6850)
	E008-E009 ACIA  (MC6850)
	E00C-E00D ACIA  (MC6850)
	E014      Drive Register
	E018-E01B FDC   (WD1771)
	E020-E027 PTM   (MC6840)
	E030-E036 ADLC  (MC6854)
	E060      Enable Z80
	F000-FFFF ROM
*/
byte proteus::read(word address)
{
byte answer;

switch (address)
	{
	/*
		Parallel Port (Optional printer port)
		MC6821 PIA (Parallel Controller) at E000-E003
		CM/P LST: (UL1:)
	*/
	case 0xE000:
	case 0xE001:
	case 0xE002:
	case 0xE003:
		answer = pia.read(address - 0xE000);
		break;

	/*
		Terminal RS232 Port (Printer port for Poly system)
		MC6850 ACIA (Serial Controller) at E004-E00
		CP/M CON:
	*/
	case 0xE004:
	case 0xE005:
		answer = acia1.read(address - 0xE004);
		break;

	/*
		Printer RS232 Port
		MC6850 ACIA (Serial Controller) at E008-E009
		CP/M LST: (LPT:)
	*/
	case 0xE008:
	case 0xE009:
		answer = acia2.read(address - 0xE008);
		break;

	/*
		Modem RS232 Port
		MC6850 ACIA (Serial Controller) at E00C-E00D
		CP/M RDR: PUN:
	*/
	case 0xE00C:
	case 0xE00D:
		answer = acia2.read(address - 0xE00C);
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
		answer = fdc[drive_select & 0x03].read(address - 0xE018);
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
		answer = timer.read(address - 0xE020);
		break;


#ifdef NEVER
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
		mc6854_channel_logging = 1;
		answer = network->read((address - 0xE030) / 2);
		mc6854_channel_logging = 0;
		break;
#endif

	/*
		Proteus Memory
	*/
	default:
		answer = memory[address];
	}

return answer;
}

/*
	PROTEUS::WRITE()
	----------------
*/
void proteus::write(word address, byte value)
{
switch (address)
	{
	/*
		Parallel Port (Optional printer port)
		MC6821 PIA (Parallel Controller) at E000-E003
		CM/P LST: (UL1:)
	*/
	case 0xE000:
	case 0xE001:
	case 0xE002:
	case 0xE003:
		pia.write(address - 0xE000, value);
		break;

	/*
		Terminal RS232 Port (Printer port for Poly system)
		MC6850 ACIA (Serial Controller) at E004, E005 and baud rate register at E006
		CP/M CON:
	*/
	case 0xE004:
	case 0xE005:
		acia1.write(address - 0xE004, value);
		break;
	/*
		Printer RS232 Port
		MC6850 ACIA (Serial Controller) at E008, E009
		CP/M LST: (LPT:)
	*/
	case 0xE008:
	case 0xE009:
		acia2.write(address - 0xE008, value);
		break;

	/*
		Modem RS232 Port
		MC6850 ACIA (Serial Controller) at E00C, E00D
		CP/M RDR: PUN:
	*/
	case 0xE00C:
	case 0xE00D:
		acia2.write(address - 0xE00C, value);
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
		fdc[drive_select & 0x03].write(address - 0xE018, value);
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
		timer.write(address - 0xE020, value);
		break;

#ifdef NEVER
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
		mc6854_channel_logging = 1;
		network->write((address - 0xE030) / 2, value);
		mc6854_channel_logging = 0;
		break;
#endif

	/*
		CPU Switch
		Switch between Z80 anc 6809
	*/
	case 0xE050:
		if (cpu_active == CPU_Z80)
			{
			cpu_active = CPU_6809;
			memcpy(z80_eprom_shadow, memory + 0xF000, sizeof(z80_eprom_shadow));		// save EPROM shadow RAM
			memcpy(memory + 0xF000, current_rom, sizeof(ROM_proteus_BIOS_31));			// put the ROM back
			}
		break;
	case 0xE060:
		if (cpu_active == CPU_6809)
			{
			cpu_active = CPU_Z80;
			memcpy(memory + 0xF000, z80_eprom_shadow, sizeof(z80_eprom_shadow));		// replace ROM with shadow RAM
			z80_reset();
			}
		break;

	/*
		Proteus Memory
	*/
	default:
		if (address < 0xE000)
			memory[address] = value;
	}
}

/*
	PROTEUS::STEP()
	---------------
*/
void proteus::step(uint64_t times)
{
long long cycles, cycles_spent, current;
uint64_t cycle = 0;

while (cycle < times)
	{
	cycles = this->cycles;
	if (cpu_active == CPU_6809)
		{
		USim::step(1);
		cycle++;
		}
	else
		cycle += z80_step();

	/*
		Send ticks to the timer
	*/
	cycles_spent = this->cycles - cycles;
	for (current = 0; current < cycles_spent; current++)
		timer.step();
	}
}
