/*
	S19_CHECK.CPP
	-------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[1024 * 1024];
uint8_t cpu_memory[0x10000];

size_t line_number = 0;

/*
	ERROR()
	-------
*/
void error(const char *message)
	{
	exit(printf("%ld: %s\n", line_number, message));
	}

/*
	HEX2_TO_VALUE()
	---------------
*/
uint8_t hex2_to_value(const char *here)
	{
	char buffer[3];

	strncpy(buffer, here, 2);
	buffer[2] = '\0';

	return strtol(buffer, NULL, 16);
	}

/*
	HEX4_TO_VALUE()
	---------------
*/
uint16_t hex4_to_value(const char *here)
	{
	char buffer[5];

	strncpy(buffer, here, 4);
	buffer[4] = '\0';

	return strtol(buffer, NULL, 16);
	}

/*
	MAIN()
	------
*/
int main(int argc, const char *argv[])
	{
	uint32_t low_address = 0xFFFF;
	uint32_t high_address = 0x0000;

	FILE *fp = fopen(argv[1], "rb");
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
		{
		uint8_t checksum;
		const char *ch = buffer;
		line_number++;
		if (*ch++ != 'S')
			error("Missing S at start of line");
		if (*ch++ != '1')
			error("Missing S1 at start of line");
		uint8_t byte_count = hex2_to_value(ch);
		ch += 2;
		uint16_t address = hex4_to_value(ch);
		ch += 4;
//printf("%04X ", address);
		checksum = byte_count + (address >> 8) + (address & 0xFF);

		for (uint8_t byte = 0; byte < byte_count - 3; byte++)
			{
			if (address < low_address)
				low_address = address;
			uint8_t value = hex2_to_value(ch);
			cpu_memory[address] = value;
			address++;
//printf("%02X ", value);
			ch += 2;
			checksum += value;
			if (address > high_address)
				high_address = address;
			}
		checksum = ~checksum;
		uint8_t stated_checksum = hex2_to_value(ch);
		ch += 2;

//		printf("\n%02X vs %02X\n", stated_checksum, checksum);
		}
	fclose(fp);

	/*
		Dump out C source code
	*/
	puts("uint8_t ROM = {");
	uint32_t here = low_address;
	while (here < high_address)
		{
		for (uint32_t line = 0; line < 16; line++)
			{
			if (here == high_address)
				printf("0x%02X", cpu_memory[here++]);
			else if (line != 15)
				printf("0x%02X, ", cpu_memory[here++]);
			else
				printf("0x%02X,", cpu_memory[here++]);
			if (here > high_address)
				break;
			}
		puts("");
		}
	puts("};");

	/*
		Binary Dump
	*/
	FILE *fp_out = fopen("EPROM.DUMP.BIN", "w+b");
	fwrite(cpu_memory, sizeof(cpu_memory), sizeof(*cpu_memory), fp_out);
	fclose(fp_out);

	return 0;
	}

