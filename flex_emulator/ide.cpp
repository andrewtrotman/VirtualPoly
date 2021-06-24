/*
	IDE.CPP
	-------
	Minimalistic IDE controller, just enough to work with FLEX

	commands for flex_vfs:
		new aspt.img t254,s254
		mount 0 aspt.dsk
		rdbootfile bootsector.bin
		wrboot 0
		import 0 FLEX/FLEX.COR
		import 0 FLEX/*
		umount 0
		quit
*/
#include <stdio.h>
#include <unistd.h>
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include <CoreFoundation/CoreFoundation.h>

#include "ide.h"

#define SECTOR_SIZE 256

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
		By default we use the disk in the app's Documents directors, but if that doesn't exist then copy it from the
		disk in the app's bundle.  This way the shipped disk is never modified, but the user's copy of that disk is.
	*/
	auto system_disk = std::filesystem::path(getenv("HOME")) / std::filesystem::path("Documents/flex.dsk");
	std::error_code status;
	if (!exists(system_disk, status))
		{
		CFBundleRef bundle = CFBundleGetMainBundle();
		#if (SECTOR_SIZE == 512)
			#define FILENAME "aspt.img"
		#else
			#define FILENAME "aspt.dsk"
		#endif
		CFURLRef url = CFBundleCopyResourceURL(bundle, CFSTR(FILENAME), NULL, NULL);
		CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		CFStringEncoding encoding_method = CFStringGetSystemEncoding();
		auto bundle_disk = std::filesystem::path(CFStringGetCStringPtr(path, encoding_method));

		std::filesystem::copy_file(bundle_disk, system_disk, std::filesystem::copy_options::skip_existing, status);

		CFRelease(url);
		CFRelease(path);
		}

	/*
		Now open the FLEX disk
	*/
	read_entire_file(system_disk.c_str(), disk);
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
			status_register = disk_number == 0 ? 0x50 : 0x40;
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
				if (disk_number == 0)
					{
					uint32_t sector = (cylinder_high_register << 16) | (cylinder_low_register << 8) | sector_number_register;
					current = ((uint8_t *)&disk[0]) + sector * SECTOR_SIZE;
					end = current + SECTOR_SIZE * sector_count_register;
					}
				else
					current = end = NULL;
				}
			break;
		}
	}
