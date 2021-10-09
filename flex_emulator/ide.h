/*
	IDE.H
	-----
	Copyright (c) 2021 Andrew Trotman

	Minimalistic IDE-like controller, just enough to work with FLEX.
	NOTE: 256 byte sectors not 512-byte sectors

	The methods used by the ROM include:
		ata_ide_command_read_sector		EQU	$20
		ata_ide_command_write_sector	EQU	$30
		ata_ide_command_identfy			EQU	$EC
		ata_ide_command_set_feature		EQU	$EF
			ata_ide_feature_8_bit		EQU	$01
*/
#pragma once

#include <string>
#include <iostream>
#include <filesystem>

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
		static const byte command_read_sector = 0x20;
		static const byte command_write_sector = 0x30;
		static const byte command_identfy = 0xEC;
		static const byte command_set_feature = 0xEF;

	private:
		byte identify_buffer[512];		// the value of the device identify block
		byte *current;					// current point on the disk
		byte *end;						// end of the current sector

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

		word disk_0_sectors_per_track;		// read the disk geometry from the FLEX disk itself.
		word disk_1_sectors_per_track;		// read the disk geometry from the FLEX disk itself.

	public:
		std::string disk_0_filename;		// local file system (MacOS, etc) filename
		std::string disk_0;					// the buffer containing the disk in drive 0
		std::string disk_1_filename;		// local file system (MacOS, etc) filename
		std::string disk_1;					// the buffer containing the disk in drive 1
		bool disk_0_did_change;				// has the disk been written to?
		bool disk_1_did_change;				// has the disk been written to?

	protected:
		std::filesystem::path get_local_filename(const std::string &filename);
		std::string move_disks_to_user_space(const std::string &filename);

	public:
		ide();
		virtual ~ide();

		void disk_load(uint8_t disk, const std::string &filename);
		void disk_save(uint8_t disk);
		bool disk_did_change(uint8_t disk);

		byte read(word address);
		void write(word address, byte value);

		std::string flex_diskname(uint8_t disk);
	};

/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const ide &simulator)
	{
	/*
		Write out the registers
	*/
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

	/*
		Write out details of how far through a disk operation we might be in.
	*/
	char which_disk = 0x80;
	qword reading_from = 0;
	qword reading_to = 0;
	if (simulator.current >= (byte *)&simulator.disk_0[0] && simulator.current < (byte *)&simulator.disk_0[simulator.disk_0.size()])
		{
		which_disk = 0;
		reading_from = simulator.current - (byte *)&simulator.disk_0[0];
		reading_to = simulator.end - (byte *)&simulator.disk_0[0];
		}
	else if (simulator.current >= (byte *)&simulator.disk_1[0] && simulator.current < (byte *)&simulator.disk_1[simulator.disk_1.size()])
		{
		which_disk = 1;
		reading_from = simulator.current - (byte *)&simulator.disk_1[0];
		reading_to = simulator.end - (byte *)&simulator.disk_1[0];
		}

	into.write(&which_disk, sizeof(which_disk));
	into.write((char *)&reading_from, sizeof(reading_from));
	into.write((char *)&reading_to, sizeof(reading_to));

	/*
		Return the stream
	*/
	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, ide &simulator)
	{
	/*
		Read the registers
	*/
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

	/*
		Read partial disk read information
	*/
	char which_disk = 0x80;
	qword reading_from = 0;
	qword reading_to = 0;

	from.read(&which_disk, sizeof(which_disk));
	from.read((char *)&reading_from, sizeof(reading_from));
	from.read((char *)&reading_to, sizeof(reading_to));

	if (which_disk == 0)
		{
		simulator.current = ((byte *)&simulator.disk_0[0]) + reading_from;
		simulator.end = ((byte *)&simulator.disk_0[0]) + reading_to;
		if (simulator.command_register == ide::command_write_sector)
			simulator.disk_0_did_change = true;
		}
	else if (which_disk == 1)
		{
		simulator.current = ((byte *)&simulator.disk_1[0]) + reading_from;
		simulator.end = ((byte *)&simulator.disk_1[0]) + reading_to;
		if (simulator.command_register == ide::command_write_sector)
			simulator.disk_1_did_change = true;
		}

	return from;
	}
