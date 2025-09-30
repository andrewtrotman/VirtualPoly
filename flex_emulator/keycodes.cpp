/*
	KEYCODES.H
	----------
	Copyright (c) 2025 Andrew Trotman
*/

#include "keycodes.h"

unsigned char poly_key_line_insert, poly_key_char_insert, poly_key_line_del, poly_key_char_del, poly_key_calc, poly_key_help;
unsigned char poly_key_exit, poly_key_back, poly_key_repeat, poly_key_next, poly_key_pause, poly_key_left, poly_key_right;
unsigned char poly_key_up, poly_key_down, poly_key_u0, poly_key_u1, poly_key_u2, poly_key_u3, poly_key_u4, poly_key_u5, poly_key_u6;
unsigned char poly_key_u7, poly_key_u8, poly_key_u9, poly_key_at, poly_key_bar, poly_key_exp, poly_key_pound, poly_key_shift_pause;
unsigned char poly_key_keypad_dot;

/*
	POLY_SET_KEYBOARD_SCAN_CODES()
	------------------------------
*/
void poly_set_keyboard_scan_codes(long poly_type)
{
if (poly_type == 1)
	{
	poly_key_line_insert = 0x10;
	poly_key_char_insert = 0x01;
	poly_key_line_del = 0x04;
	poly_key_char_del = 0x02;
	poly_key_calc = 0x5E;
	poly_key_help = 0x7E;
	poly_key_exit = 0x7B;
	poly_key_back = 0x5B;
	poly_key_repeat = 0x7D;
	poly_key_next = 0x5D;
	poly_key_pause = 0x1B;
	poly_key_left = 0x08;
	poly_key_right = 0x09;
	poly_key_up = 0x0B;
	poly_key_down = 0x0A;
	poly_key_u0 = 0x5F;
	poly_key_u1 = 0x14;
	poly_key_u2 = 0x15;
	poly_key_u3 = 0x16;
	poly_key_u4 = 0x17;
	poly_key_u5 = 0x19;
	poly_key_u6 = 0x1A;
	poly_key_u7 = 0x7F;
	poly_key_u8 = 0x1D;
	poly_key_u9 = 0x06;
	poly_key_at = 0x7C;
	poly_key_bar = 0x40;
	poly_key_exp = 0x60;
	poly_key_pound = 0x5C;

	// the following are not on the Poly 1 keyboard!
	poly_key_shift_pause = 0x00;
	poly_key_keypad_dot = 0x00;
	}
else if (poly_type == 2)
	{
	poly_key_line_insert = 0x7B;
	poly_key_char_insert = 0x5B;
	poly_key_line_del = 0x5E;
	poly_key_char_del = 0x7E;
	poly_key_calc = 0x12;
	poly_key_help = 0x1D;
	poly_key_exit = 0x5C;
	poly_key_back = 0x7C;
	poly_key_repeat = 0x7D;
	poly_key_next = 0x5D;
	poly_key_pause = 0x0F;
	poly_key_left = 0x1F;
	poly_key_right = 0x11;
	poly_key_up = 0x08;
	poly_key_down = 0x0B;
	poly_key_u0 = 0x16;
	poly_key_u1 = 0x15;
	poly_key_u2 = 0x18;
	poly_key_u3 = 0x01;
	poly_key_u4 = 0x7F;
	poly_key_u5 = 0x19;
	poly_key_u6 = 0x06;
	poly_key_u7 = 0x1A;
	poly_key_u8 = 0x0A;
	poly_key_u9 = 0x17;
	poly_key_at = 0x09;
	poly_key_bar = 0x40;
	poly_key_exp = 0x60;
	poly_key_pound = 0x13;

	// Poly 2 keyboard only
	poly_key_shift_pause = 0x0E;
	poly_key_keypad_dot = 0x1E;
	}
}
