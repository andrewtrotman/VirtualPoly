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
#include <iostream>

#include "typedefs.h"

/*
	class IDE
	---------
*/
class ide
	{
	friend std::ostream &operator<<(std::ostream &into, const ide &simulator);
	friend inline std::istream &operator>>(std::istream &from, ide &simulator);

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

		void save_disk();
	};

/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const ide &simulator)
	{
	into.write((char *)&simulator.data_register, sizeof(simulator.data_register));
	into.write((char *)&simulator.feature_register, sizeof(simulator.feature_register));
	into.write((char *)&simulator.error_register, sizeof(simulator.error_register));
	into.write((char *)&simulator.sector_count_register, sizeof(simulator.sector_count_register));
	into.write((char *)&simulator.sector_number_register, sizeof(simulator.sector_number_register));
	into.write((char *)&simulator.cylinder_low_register, sizeof(simulator.cylinder_low_register));
	into.write((char *)&simulator.cylinder_high_register, sizeof(simulator.cylinder_high_register));
	into.write((char *)&simulator.disk_head_register, sizeof(simulator.disk_head_register));
	into.write((char *)&simulator.status_register, sizeof(simulator.status_register));
	into.write((char *)&simulator.command_register, sizeof(simulator.command_register));

	qword reading_from = (simulator.current == NULL ? std::numeric_limits<decltype(reading_from)>::max() : simulator.current - (byte *)&simulator.disk[0]);
	into.write((char *)&reading_from, sizeof(reading_from));

	qword reading_to = (simulator.end == NULL ? std::numeric_limits<decltype(reading_to)>::max() : simulator.end - (byte *)&simulator.disk[0]);
	into.write((char *)&reading_to, sizeof(reading_to));

	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, ide &simulator)
	{
	from.read((char *)&simulator.data_register, sizeof(simulator.data_register));
	from.read((char *)&simulator.feature_register, sizeof(simulator.feature_register));
	from.read((char *)&simulator.error_register, sizeof(simulator.error_register));
	from.read((char *)&simulator.sector_count_register, sizeof(simulator.sector_count_register));
	from.read((char *)&simulator.sector_number_register, sizeof(simulator.sector_number_register));
	from.read((char *)&simulator.cylinder_low_register, sizeof(simulator.cylinder_low_register));
	from.read((char *)&simulator.cylinder_high_register, sizeof(simulator.cylinder_high_register));
	from.read((char *)&simulator.disk_head_register, sizeof(simulator.disk_head_register));
	from.read((char *)&simulator.status_register, sizeof(simulator.status_register));
	from.read((char *)&simulator.command_register, sizeof(simulator.command_register));

	qword reading_from;
	from.read((char *)&reading_from, sizeof(reading_from));
	simulator.current = ((byte *)&simulator.disk[0]) + reading_from;

	qword reading_to;
	from.read((char *)&reading_to, sizeof(reading_to));
	simulator.end = ((byte *)&simulator.disk[0]) + reading_to;

	return from;
	}
