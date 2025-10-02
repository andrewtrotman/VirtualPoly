/*
	WD1771.CPP
	----------
	Copyright (c) 2025 Andrew Trotman
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include "wd1771.h"

/*
	WD1771::WD1771()
	----------------
*/
wd1771::wd1771(void)
{
strcpy(dsk_filename, "disk0.dsk");			// default filename
door_opened = false;

disk = NULL;
bytepos = max_sector = max_track = track = sector = side = 0;
sector_size = 0x100;
direction = last_type = 1;
busy = multiple = write_protected = false;
current_command = data_register = 0x00;
status_calls = 0;
}

/*
	WD1771::READ_ENTIRE_FILE()
	--------------------------
*/
byte *wd1771::read_entire_file(const char *filename, long long *size)
{
FILE *fp;
struct stat stats;
byte *buffer;

*size = 0;
if (stat(filename, &stats) != 0)
	return NULL;

if ((fp = fopen(filename, "rb")) == NULL)
	return NULL;

buffer = new byte [stats.st_size + 1];
if (fread(buffer, stats.st_size, 1, fp) != 1)
	{
	delete [] buffer;
	buffer = NULL;
	}
else
	buffer[stats.st_size] = '\0';

fclose(fp);

*size = stats.st_size;
return buffer;
}

/*
	WD1771::LOAD_DSK()
	------------------
*/
unsigned char *wd1771::load_dsk(const char *filename, long *error_code)
{
unsigned char *pos, *into;

delete [] disk;

if ((disk = read_entire_file(filename, &size_of_disk_in_bytes)) == NULL)
	{
	*error_code = ERROR_CANNOT_READ;
	return NULL;
	}

if (size_of_disk_in_bytes == 295680)			// FLEX or POLYSYS 8" disk (76 tracks of 15 sectors)
	operating_system = FLEX;
else if (size_of_disk_in_bytes == 591360)		// FLEX or POLYSYS 8" disk (76 tracks of 30 sectors (double sided))
	operating_system = FLEX;
else if (size_of_disk_in_bytes == 89600)		// FLEX 5.25" disk (34 tracks of 10 sectors)
	operating_system = FLEX;
else if (size_of_disk_in_bytes == 179200)		// FLEX 5.25" disk (34 tracks of 20 sectors (double sided))
	operating_system = FLEX;
else if (size_of_disk_in_bytes == 102400)		// FLEX 5.25" disk (39 tracks of 10 sectors)
	operating_system = FLEX;
else if (size_of_disk_in_bytes == 204800)		// FLEX 5.25" disk (39 tracks of 20 sectors (double sided))
	operating_system = FLEX;
else if (size_of_disk_in_bytes == 622592)		// CP/M 3" disk (76 tracks of 16 sectors)
	{
	max_track = 76;
	max_sector = 0x10;
	sector_size = 0x200;
	operating_system = CPM;
	}
else if (size_of_disk_in_bytes == 630784)		// CP/M 3" disk and 8" disk (77 tracks of 16 sectors)
	{
	max_track = 77;
	max_sector = 0x10;
	sector_size = 0x200;
	operating_system = CPM;
	}
else
	{
	*error_code = ERROR_UNKNOWN_FORMAT;
	return NULL;								// unknown format
	}

if (operating_system == FLEX)
	{
	max_track = disk[0x200 + 38];		// from the FLEX SIR
	max_sector = disk[0x200 + 39];		// from the FLEX SIR
	sector_size = 0x100;
	}

track = side = 0;
sector = 1;
direction = 1;
bytepos = 0;
last_type = 1;
busy = false;
multiple = false;
data_register = 0x00;
current_command = 0x00;

if (operating_system == CPM)
	{
	strcpy((char *)volume_name, "CP/M Disk");
	volume_number = 0;
	}
else if (operating_system == FLEX)
	{
	into = volume_name;

	volume_number = (((unsigned)disk[0x200 + 27]) << 8) + ((unsigned)disk[0x200 + 28]);
	into += snprintf((char *)into, sizeof(volume_name), "#%ld (", volume_number);

	for (pos = disk + 0x200 + 16; pos < disk + 0x200 + 26; pos++)
		if (isprint(*pos))
			*into++ = *pos;
		else
			break;
	
	snprintf((char *)into, 1, ")");
	}

*error_code = ERROR_NONE;
return volume_name;
}

/*
	WD1771::MOUNT_DISK()
	--------------------
*/
unsigned char *wd1771::mount_disk(const char *dsk_filename, long *error_code)
{
struct stat stats;

door_opened = true;
if (stat(dsk_filename, &stats) != 0)							// problem with the file
	{
	*error_code = ERROR_CANNOT_STAT;
	return NULL;
	}

write_protected =  stats.st_mode & S_IWRITE ? false : true;

*error_code = ERROR_NONE;
return load_dsk(dsk_filename, error_code);
}


/*
	WD1771::SET_BYTE()
	------------------
*/
void wd1771::set_byte(unsigned char val)
{
long pos;

if (disk == NULL)		// no disk in drive
	return;

status_calls = 0;

data_register = val;			// necessary for seek operations

if (writing)		// write to disk
	{
	if  (write_protected)
		 bytepos = writing = busy = multiple = false;
	else
		{
		pos = (track * max_sector * sector_size) + ((sector - 1) * sector_size) + bytepos;

		bytepos++;

		if (bytepos >= sector_size)
			{
			bytepos = 0;
			sector++;
			if (!multiple)
				writing = busy = false;
			}

		if (sector > max_sector)
			{
			writing = busy = multiple = false;
			sector = 1;
			}

		if (pos < size_of_disk_in_bytes)
			disk[pos] = val;
		}
	}
}

/*
	WD1771::GET_BYTE()
	------------------
*/
unsigned char wd1771::get_byte(void)
{
long pos;

if (disk == NULL)		// no disk in drive
	return 0;

status_calls = 0;
pos = (track * max_sector * sector_size) + ((sector - 1) * sector_size) + bytepos;

bytepos++;

if (bytepos >= sector_size)
	{
	bytepos = 0;
	sector++;
	if (!multiple)
		busy = false;
	}

if (sector > max_sector)
	{
	busy = false;
	multiple = false;
	sector = 1;
	}

if (pos >= size_of_disk_in_bytes)
	return 0;
else
	return disk[pos];
}

/*
  WD1771::GET_STATUS()
	-------------------
*/
unsigned char wd1771::get_status(void)
{
unsigned char answer;

if (++status_calls > 0x10)
	busy = false;				// because we're a FLEX verify which is "dodgy"

answer = 0x00;			// initialise
answer |= disk == NULL ? 0x80 : 0x00;		// not ready because there is no disk in the drive
answer |= write_protected ? 0x40 : 0x00;		// write protect bit

/*
	Type 1: motor has spun up
	Type 2,3: record type (0=data mark, 1 = deleted data mark)
*/
if (last_type == 1)
	answer |= 0x20;		// motor has spun up
else	// type 2 or 3
	answer |= 0x00;		// no deleted data marks allowed

answer |= 0x00;		// track, sector, or side not-found

answer |= 0x00;		// CRC error

/*
	Type 1: TR00 signal (active low)
	Type 2,3: lost data
*/
if (last_type == 1)
	{
	if (track == 0)
		answer |= 0x00;		// At track 00
	else
		answer |= 0x04;		// not at track 00
	}
else
	answer |= 0x00;		// lost data

/*
	Type 1: status of the IP (index position) signal (active low)

	Type 2,3:DRQ (data request) 1->CPU proceed, 0->CPU block
*/
if (last_type == 1)
	{
	if (sector == 0)
		answer |= 0x00;		// At sector 0
	else
		answer |= 0x02;		// not at sector 0
	}
else
	{
	if (busy)
		answer |= 0x02;		// data can be read from read / write
	else
		answer |= 0x00;		// data cannot be read from read / write
	}

if (busy)
	{
	if (writing)
		answer |= 0x00;		// when we write we swallow the byte immediately (so never busy)
	else
		answer |= busy ? 0x01 : 0x00;		// busy in a command
	}

return answer;
}

/*
	WD1771::SET_COMMAND()
	---------------------
*/
void wd1771::set_command(unsigned char val)
{
status_calls = 0;
current_command = val;
busy = false;
writing = false;
/*
	Type I commands
*/
last_type = 1;
if ((val & 0xF0) == 0x00)	// restore
	track = 0;
else if ((val & 0xF0) == 0x10)	// seek
	track = data_register;
else if ((val & 0xE0) == 0x20)	// step
	{
	if (val & 0x1)
		track = (unsigned char)(track + direction);
	}
else if ((val & 0xE0) == 0x40)	// step in
	{
	if (val & 0x1)
		track = track + 1;
	direction = 1;
	}
else if ((val & 0xE0) == 0x60)	// step out
	{
	if (val & 0x1)
		track = track - 1;
	direction = -1;
	}
/*
	Type II commands
*/
else if ((val & 0xE0) == 0x80)	// read
	{
	bytepos = 0;
	last_type = 2;
	busy = true;
	multiple = ((val & 0x10) == 0) ? false : true;
	}
else if ((val & 0xE0) == 0xA0)	// write
	{
	bytepos = 0;
	last_type = 2;
	busy = true;
	writing = true;
	multiple = ((val & 0x10) == 0) ? false : true;
	}
/*
	Type III commands	(unimplemented)
*/
else if ((val & 0xF0) == 0xC0)	// read Address
	last_type = 3;
else if ((val & 0xF0) == 0xD0)	// read Track
	last_type = 3;
else if ((val & 0xF0) == 0xF0)	// write Track
	last_type = 3;
/*
	Type IV commands (unimplemented)
*/
else if ((val & 0xF0) == 0xD0)	// force interrupt
	{
	}
}

/*
	WD1771::READ()
	--------------
*/
byte wd1771::read(word address)
{
switch (address)
	{
	case 0x0000:		// status register
		return get_status();
	case 0x0001:		// Track regster
		return get_track();
	case 0x0002:		// Sector register
		return get_sector();
	case 0x0003:		// Data register
		return get_byte();
	}
return 0;
}

/*
	WD1771::WRITE()
	---------------
*/
void wd1771::write(word address, byte value)
{
switch (address)
	{
	case 0x0000:		// Command register
		set_command(value);
		break;
	case 0x0001:		// Track regster
		set_track(value);
		break;
	case 0x0002:		// Sector register
		set_sector(value);
		break;
	case 0x0003:		// Data register
		set_byte(value);
		break;
	}
}
