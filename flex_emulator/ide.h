/*
	IDE.H
	-----
	Minimalistic IDE controller, just enough to work with FLEX

	The methods used by the ROM include:
		ata_ide_command_read_sector	EQU	$20
		ata_ide_command_write_sector	EQU	$30
		ata_ide_command_identfy			EQU	$EC
		ata_ide_command_set_feature	EQU	$EF
			ata_ide_feature_8_bit			EQU	$01
*/
#pragma once

#include <string>

#include "typedefs.h"

/*
	class IDE
	---------
*/
class ide
	{
	private:
		const byte command_read_sector = 0x20;
		const byte command_write_sector = 0x30;
		const byte command_identfy = 0xEC;
		const byte command_set_feature = 0xEF;

	private:
		byte identify_buffer[512];	// the value of the device identify block
		byte *current;					// current point on the disk
		byte *end;						// end of the current sector
		std::string disk;				// the buffer containing the disk

		byte data_register;
		byte feature_register;
		byte error_register;
		byte sector_count_register;
		byte sector_number_register;
		byte cylinder_low_register;
		byte cylinder_high_register;
		byte disk_head_register;
		byte status_register;
		byte command_register;

	public:
		ide();
		virtual ~ide();

		byte read(word address);
		void write(word address, byte value);
	};
