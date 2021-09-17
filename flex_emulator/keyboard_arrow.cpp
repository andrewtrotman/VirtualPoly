/*
	KEYBOARD_ARROW.CPP
	------------------
*/
#include <iostream>
#include "keyboard_arrow.h"

/*
	KEYBOARD_ARROW::KEYBOARD_ARROW()
	--------------------------------
*/
keyboard_arrow::keyboard_arrow(std::deque<byte> &keystream):
	keystream(keystream),
	announce(false)
	{
	/* Nothing */
	}

/*
	KEYBOARD_ARROW::~KEYBOARD_ARROW()
	---------------------------------
*/
keyboard_arrow::~keyboard_arrow()
	{
	/* Nothing */
	}


uint8_t where_in_sequence = 0;
static const uint8_t byte_sequence[] =
	{
	/*
		01/01 -> '2'
		03/07 -> '7'
		03/03 -> '5'
		03/01 -> '3'
		03/00 -> '1'
		07/03 -> 'R'
		0F/01 -> 'E'
		0F/00 -> 'Q'

	;	A = $0F -> Contol key
	;	A = $1F -> Toggle caps lock
	;	A = $3F -> Graphics key?
	;	A = $7F -> Shift key
	*/
	0x03,			// key set
	0x07,			// key within set
	0x00,			// key modifier (shift, etc) (0x7F -> Shift)
	0x03,			// key set
	0x07,			// key within set
	0x00,			// key modifier (shift, etc) (0x7F -> Shift)

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

/*
	KEYBOARD_ARROW::READ()
	----------------------
*/
uint8_t keyboard_arrow::read(uint16_t address)
	{	
	uint8_t answer = 0;

	if (reset)
		answer = 0x80;
	else if (announce)
		{
		if (keystream.size() != 0)
			answer = 0x88;
		else
			answer = 0x00;
		announce = false;
		}
	else
		{
		uint8_t check = where_in_sequence % 3;
		if (check == 0 || check == 2)
			{
			if ((bit_check & byte_sequence[where_in_sequence]) != byte_sequence[where_in_sequence])
				{
				answer = 0x00;
				where_in_sequence++;
				}
			else
				answer = 0xFF;
			}
		else
			answer = byte_sequence[where_in_sequence++];
		}

	printf("%04X: Read %04X = %02X\n", start_of_instruction, address + 0xE008, answer);

	return answer;
	}

/*
	KEYBOARD_ARROW::WRITE()
	-----------------------
*/
void keyboard_arrow::write(uint16_t address, uint8_t value)
	{
	printf("%04X: Write %02X to %04X\n", start_of_instruction, value, address + 0xE008);
	if (address == 4)
		{
		if (value == 0x9F)
			{
			reset = true;
			where_in_sequence = 0xFF;
			}
		else if (value == 0x00 && where_in_sequence != 0)
			{
			reset = false;
			announce = true;
			where_in_sequence = 0;
			}
		else
			bit_check = value;
		}
	}

/*
	KEYBOARD_ARROW::TRANSLATE()
	---------------------------
*/
void keyboard_arrow::queue_key_press(byte key)
	{
	const char plain_keys[8][8] =
		{
		"\u001B24680-",
		"13579:^",
		"\x09wryip]",
		"qetuo[\x0A",
		"adgjl@\x7F",
		"\x01sfhk;\\",
		"zcbm. \x0D",
		"xvn,/\u0008\u0000"
		};

	/*
		key presses without any modifiers
	*/
	for (uint16_t row = 0; row < 8; row++)
		{
		const char *pos = strchr(plain_keys[row], key);
		if (pos != NULL)
			{
			std::cout << "row: " << row << " col: " << pos - plain_keys[row] << " mod: " << 0 << "\n";
			}
		}

	const char shift_keys[8][8] =
		{
		"\u001B\"$&(0-",
		"!#%'):^",
		"\x09WRYIP=",
		"QETUO;\x0A",
		"ADGJL`\x7F",
		"\x01SFHK*<",
		"ZCBM> \x0D",
		"XVN</\u0008\u0001"
		};
	for (uint16_t row = 0; row < 8; row++)
		{
		const char *pos = strchr(plain_keys[row], key);
		if (pos != NULL)
			{
			std::cout << "row: " << row << " col: " << pos - plain_keys[row] << " mod: " << 0x7F << "\n";
			}
		}

	}
