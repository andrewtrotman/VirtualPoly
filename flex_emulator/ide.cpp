/*
	IDE.CPP
	-------
	Minimalistic IDE controller, just enough to work with FLEX
*/

#include "ide.h"

/*
	IDE::IDE()
	----------
*/
ide::ide()
	{
	data_register = 0;
	feature_register = 0;
	error_register = 0;
	sector_count_register = 0;
	sector_number_register = 0;
	cylinder_low_register = 0;
	cylinder_high_register = 0;
	disk_head_register = 0;
	status_register = 0x40;			// not BUSY, but DRDY
	command_register = 0;

	for (byte *into = identify_buffer; into < identify_buffer + sizeof(identify_buffer); into++)
		*into = 'Z';
	current = identify_buffer;
	end = identify_buffer + sizeof(identify_buffer);
	}

/*
	IDE::~IDE()
	-----------
*/
ide::~ide()
	{
	/* Nothing */
	}

/*
	IDE::READ()
	-----------
*/
byte ide::read(word address)
	{
	byte value = 0;

	switch (address)
		{
		case 0:
			if (command_register == command_identfy || command_register == command_read_sector)
				if (current < end)
					value = *current++;
			break;
		case 1:
			value = error_register;
			break;
		case 2:
			value = sector_count_register;
			break;
		case 3:
			value = sector_number_register;
			break;
		case 4:
			value = cylinder_low_register;
			break;
		case 5:
			value = cylinder_high_register;
			break;
		case 6:
			value = disk_head_register;
			break;
		case 7:
			value = status_register;
			break;
		default:
			return 0;
		}
	return value;
	}

/*
	IDE::WRITE()
	------------
*/
void ide::write(word address, byte value)
	{
	switch (address)
		{
		case 0:
			data_register = value;
			break;
		case 1:
			feature_register = value;
			break;
		case 2:
			sector_count_register = value;
			break;
		case 3:
			sector_number_register = value;
			break;
		case 4:
			cylinder_low_register = value;
			break;
		case 5:
			cylinder_high_register = value;
			break;
		case 6:
			 disk_head_register = value;
			break;
		case 7:
			command_register = value;
			if (command_register == command_identfy)
				{
				current = identify_buffer;
				end = identify_buffer + 512;
				}
			else if (command_register == command_read_sector)
				{
				uint32_t disk_number = (disk_head_register >> 4) & 0x01;
				status_register = disk_number == 0 ? 0x40 : 0x50;
				uint32_t sector = (cylinder_high_register << 16) | (cylinder_low_register << 8) | sector_number_register;
				current = disk + sector * 512;
				end = current + 512 * sector_count_register;
				}
			break;
		}
	}
