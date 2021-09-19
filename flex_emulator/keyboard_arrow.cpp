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
keyboard_arrow::keyboard_arrow():
	mode(RESET),
	where_in_sequence(0)
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

#ifdef NEVER
static const uint8_t byte_sequence[] =
	{
	/*
		01/01 -> '2'
		03/0F -> '9'
Y		03/07 -> '7'
Y		03/03 -> '5'
Y		03/01 -> '3'
Y		03/00 -> '1'
		07/03 -> 'R'
Y		0F/01 -> 'E'
Y		0F/00 -> 'Q'

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
#endif

/*
	KEYBOARD_ARROW::READ()
	----------------------
*/
uint8_t keyboard_arrow::read(uint16_t address)
	{
	uint8_t answer = 0;

	if (address == 0x04)
		answer = 0xFF;
	else
		{
		if (mode == RESET)
			answer = 0x80;
		else if (mode == ANNOUNCE)
			{
			if (keystream.size() == 0)
				answer = 0xFF;
			else
				{
				answer = 0x00;
				mode = REPORT;
				}
			}
		else
			{
			uint8_t check = where_in_sequence % 3;
			if (check == 0 || check == 2)
				{
				if (keystream.size() == 0)
					answer = 0xFF;
				else if ((bit_check & keystream.front()) != keystream.front())
					{
					answer = 0x00;
					keystream.pop_front();
					where_in_sequence++;
					if (keystream.size() % 3 == 0)
						mode = ANNOUNCE;
					}
				else
					answer = 0xFF;
				}
			else
				{
				if (keystream.size() == 0)
					answer = 0;
				else
					{
					answer = keystream.front();
					keystream.pop_front();
					where_in_sequence++;
					}
				}
			}
		}
	return answer;
	}

/*
	KEYBOARD_ARROW::WRITE()
	-----------------------
*/
void keyboard_arrow::write(uint16_t address, uint8_t value)
	{
	if (address == 4)
		{
		if (value == 0x9F)
			mode = RESET;
		else if ((value == 0x00) && (keystream.size() % 3 == 0) && mode != REPORT)
			{
			mode = ANNOUNCE;
			where_in_sequence = 0;
			}
		else
			{
			mode = REPORT;
			bit_check = value;
			}
		}
	}

/*
	KEYBOARD_ARROW::TRANSLATE()
	---------------------------
	Incoming:
	DEL = 0x08
	ESC = 0x1B
	TAB = 0x09

	BS = 0x7F
	UP = 0x0B

	down arrow = ^J
	left arrow = ^H
	right arrow = ^I
	up arrow = ^K
*/
void keyboard_arrow::queue_key_press(byte key)
	{
	const uint8_t bits[] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
	const char plain_keys[8][8] =
		{
		"\u001B24680-",
		"13579:^",
		"\x09WRYIP]",
		"QETUO[\x0A",
		"ADGJL@\x7F",
		"\xFFSFHK;\\",
		"ZCBM. \x0D",
		"\u001bXVN,/\u0008"
		};

	/*
		key presses without any modifiers
	*/
	for (uint16_t row = 0; row < 8; row++)
		{
		const char *pos = strchr(plain_keys[row], key);
		if (pos != NULL)
			{
			std::cout << std::hex;
			std::cout << "row: " << (int)bits[row + 1] << " col: " << (int)bits[pos - plain_keys[row]] << " mod: " << 0x01 << "\n";
			std::cout << std::dec;
			keystream.push_back(bits[row + 1]);
			keystream.push_back(bits[pos - plain_keys[row]]);
			keystream.push_back(0x01);
			keystream.push_back(bits[row + 1]);
			keystream.push_back(bits[pos - plain_keys[row]]);
			keystream.push_back(0x01);
			return;
			}
		}

	const char shift_keys[8][8] =
		{
		"\u001B\"$&(0=",
		"!#%')*~",
		"\x09wryip}",
		"qetuo{\x0A",
		"adgjl`\x7F",
		"\xFFsfhk+|",
		"zcbm> \x0D",
		"\u001bxvn<?\u0008"
		};
	for (uint16_t row = 0; row < 8; row++)
		{
		const char *pos = strchr(shift_keys[row], key);
		if (pos != NULL)
			{
			std::cout << std::hex;
			std::cout << "row: " << (int)bits[row + 1] << " col: " << (int)bits[pos - shift_keys[row]] << " mod: " << 0x7F << "\n";
			std::cout << std::dec;
			keystream.push_back(bits[row + 1]);
			keystream.push_back(bits[pos - shift_keys[row]]);
			keystream.push_back(0xFF);
			keystream.push_back(bits[row + 1]);
			keystream.push_back(bits[pos - shift_keys[row]]);
			keystream.push_back(0xFF);
			return;
			}
		}

	const char control_keys[8][8] =
		{
		"       ",
		"       ",
		" \x17\x12\x19\x09\x10 ",
		"\x11\x05\x14\x15\x0f  ",
		"\x01\x04\x07\x0a\x0c  ",
		" \x13\x06\x08\x0b  ",
		"\x1a\x03\x02\x0d   ",
		" \x18\x16\x0e   "
		};

	for (uint16_t row = 0; row < 8; row++)
		{
		const char *pos = strchr(control_keys[row], key);
		if (pos != NULL)
			{
			std::cout << std::hex;
			std::cout << "row: " << (int)bits[row + 1] << " col: " << (int)bits[pos - control_keys[row]] << " mod: " << 0x1F << "\n";
			std::cout << std::dec;
			keystream.push_back(bits[row + 1]);
			keystream.push_back(bits[pos - control_keys[row]]);
			keystream.push_back(0x1F);
			keystream.push_back(bits[row + 1]);
			keystream.push_back(bits[pos - control_keys[row]]);
			keystream.push_back(0x1F);
			return;
			}
		}

	}
