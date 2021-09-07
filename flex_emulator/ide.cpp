/*
	IDE.CPP
	-------
	Copyright (c) 2021 Andrew Trotman

	Minimalistic IDE-like controller, just enough to work with FLEX.
	NOTE: 256 byte sectors not 512-byte sectors
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <CoreFoundation/CoreFoundation.h>

#include "ide.h"

/*
	READ_ENTIRE_FILE()
	------------------
*/
size_t read_entire_file(const std::string &filename, std::string &into)
	{
	FILE *fp;							// "C" pointer to the file
	struct stat details;				// file system's details of the file
	size_t file_length = 0;			// length of the file in bytes

	if ((fp = fopen(filename.c_str(), "rb")) != nullptr)
		{
		if (fstat(fileno(fp), &details) == 0)
			if ((file_length = details.st_size) != 0)
				{
				into.resize(file_length);
				if (fread(&into[0], details.st_size, 1, fp) != 1)
					into.resize(0);
				}
		fclose(fp);
		}

	return file_length;
	}

/*
	WRITE_ENTIRE_FILE()
	-------------------
*/
bool write_entire_file(const std::string &filename, const std::string &buffer)
	{
	FILE *fp;						// "C" file to write to

	if ((fp = fopen(filename.c_str(), "wb")) == nullptr)
		return false;

	size_t success = fwrite(&buffer[0], buffer.size(), 1, fp);

	fclose(fp);

	return success == 1 ? true : false;
	}

/*
	IDE::IDE()
	----------
*/
ide::ide()
	{
	/*
		Set up the controller
	*/
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

	/*
		Create a 512-byte buffer as that is the result of a disk identify command
	*/
	for (byte *into = identify_buffer; into < identify_buffer + sizeof(identify_buffer); into++)
		*into = 'Z';
	current = identify_buffer;
	end = identify_buffer + sizeof(identify_buffer);

	/*
		By default we use the disk in the app's Documents folder, but if that doesn't exist then copy it from the
		disk in the app's bundle.  This way the shipped disk is never modified, but the user's copy of that disk is.
	*/
	move_disks_to_user_space("flex.dsk");
	move_disks_to_user_space("user.dsk");

	load_disk(0, get_local_filename("flex.dsk").string().c_str());
	load_disk(1, get_local_filename("user.dsk").string().c_str());
	}

/*
	IDE::GET_LOCAL_FILENAME()
	-------------------------
*/
std::filesystem::path ide::get_local_filename(const std::string &filename)
	{
	return std::filesystem::path(getenv("HOME")) / std::filesystem::path("Documents") / std::filesystem::path(filename);
	}

/*
	IDE::MOVE_DISKS_TO_USER_SPACE()
	-------------------------------
*/
std::string ide::move_disks_to_user_space(const std::string &filename)
	{
	auto full_filename = get_local_filename(filename);
	std::error_code status;
//	if (!exists(full_filename, status))
		{
		CFBundleRef bundle = CFBundleGetMainBundle();
		CFStringRef munged_name = CFStringCreateWithCString(NULL, filename.c_str(), kCFStringEncodingUTF8);
		CFURLRef url = CFBundleCopyResourceURL(bundle, munged_name, NULL, NULL);
		CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		auto bundle_disk = std::filesystem::path(CFStringGetCStringPtr(path, CFStringGetSystemEncoding()));

//		std::filesystem::copy_file(bundle_disk, full_filename, std::filesystem::copy_options::skip_existing, status);
		std::filesystem::copy_file(bundle_disk, full_filename, std::filesystem::copy_options::overwrite_existing, status);

		CFRelease(url);
		CFRelease(path);
		CFRelease(munged_name);
		}
	return full_filename.string();
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
	IDE::LOAD_DISK()
	----------------
*/
void ide::load_disk(uint8_t disk, const std::string &filename)
	{
	if (disk == 0)
		{
		disk_0_filename = filename;
		read_entire_file(filename.c_str(), disk_0);
		disk_0_sectors_per_track = disk_0[0x227];
		}
	else		// disk == 1
		{
		disk_1_filename = filename;
		read_entire_file(filename.c_str(), disk_1);
		disk_1_sectors_per_track = disk_0[0x227];
		}
	}

/*
	IDE::SAVE_DISK()
	----------------
*/
void ide::save_disk(uint8_t disk)
	{
	if (disk == 0)
		if	(disk_0_filename.size() != 0)
			write_entire_file(disk_0_filename, disk_0);

	if (disk == 1)
		if	(disk_1_filename.size() != 0)
			write_entire_file(disk_1_filename, disk_1);
	}

/*
	IDE::FLEX_DISKNAME()
	--------------------
*/
std::string ide::flex_diskname(uint8_t disk)
	{
	if (disk == 0)
		return std::string(&disk_0[0x210], 0x0B);
	else		// disk == 1
		return std::string(&disk_1[0x210], 0x0B);
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
				{
				if (current < end)
					value = *current++;
				if (current == end)
					status_register = 0x50;
				}
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
			if (command_register == command_write_sector)
				{
				if (current < end)
					*current++ = value;
				if (current == end)
					status_register = 0x50;
				}
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
			{
			disk_head_register = value;
			uint32_t disk_number = (disk_head_register >> 4) & 0x01;
			status_register = (disk_number == 0 || disk_number == 1) ? 0x50 : 0x40;
			break;
			}
		case 7:
			command_register = value;
			if (command_register == command_identfy)
				{
				current = identify_buffer;
				end = identify_buffer + 512;
				}
			else if (command_register == command_read_sector || command_register == command_write_sector)
				{
				uint32_t disk_number = (disk_head_register >> 4) & 0x01;
				uint32_t block = (cylinder_high_register << 16) | (cylinder_low_register << 8) | sector_number_register;

				/*
					Convert from the block number the ROM generates to a track / sector number.
				*/
				uint32_t sector = block % 254;
				uint32_t track = block / 254;

				/*
					Now convert to a disk offset location.
				*/
				if (disk_number == 0)
					{
					uint32_t actual = track * disk_0_sectors_per_track + sector;
					current = ((uint8_t *)&disk_0[0]) + actual * 256;
					}
				else // 	disk_number == 1
					{
					uint32_t actual = track * disk_1_sectors_per_track + sector;
					current = ((uint8_t *)&disk_1[0]) + actual * 256;
					}
					
				/*
					And the end if 256 bytes later because FLEX sectors are 256 bytes in size
				*/
				end = current + 256 * sector_count_register;
				}
			break;
		}
	}
