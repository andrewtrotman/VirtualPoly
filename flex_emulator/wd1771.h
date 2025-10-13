/*
	WD1771.H
	--------
	Copyright (c) 2025 Andrew Trotman
*/
#pragma once

#include <sys/syslimits.h>

#include "typedefs.h"

/*
	class WD1771
	------------
*/
class wd1771
{
protected:
	enum { FLEX, CPM };

public:
	enum { ERROR_NONE, ERROR_CANCEL, ERROR_CANNOT_STAT, ERROR_CANNOT_READ, ERROR_UNKNOWN_FORMAT };

public:
	unsigned char track, sector, side, data_register;
	long direction;
	unsigned char *disk;
	long operating_system;
	long bytepos;
	long max_track, max_sector, sector_size;
	long last_type;
	long busy, writing, status_calls;
	long multiple;
	unsigned char current_command;
	long write_protected;
	long door_opened;

	bool disk_contents_changed;
	long long size_of_disk_in_bytes;
	char dsk_filename[PATH_MAX];			// on MacOS this appears to be 1024
	unsigned char volume_name[80];
	long volume_number;

protected:
	void set_track(unsigned char val) { track = val; }
	unsigned char get_track(void) { return track; }

	void set_sector(unsigned char val) { sector = val; }
	unsigned char get_sector(void) { return sector; }

	void set_byte(unsigned char val);
	unsigned char get_byte(void);

	void set_command(unsigned char val);
	unsigned char get_status(void);

	byte *read_entire_file(const char *filename, long long *size);
	unsigned char *load_dsk(const char *filename, long *error_code);

public:
	wd1771();
	~wd1771() { unmount_disk(); delete [] disk; }

	unsigned char *mount_disk(const char *dsk_filename, long *error_code);
	long unmount_disk(void);

	void clear_door_opened(void) { door_opened = 0; }
	long get_door_opened(void) { return door_opened; }

	byte read(word address);
	void write(word address, byte value);
};
