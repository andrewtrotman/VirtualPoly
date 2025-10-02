/*
	KEYCODES.H
	----------
	Copyright (c) 2025 Andrew Trotman
*/

#pragma once

extern unsigned char poly_key_line_insert, poly_key_char_insert, poly_key_line_del, poly_key_char_del, poly_key_calc, poly_key_help;
extern unsigned char poly_key_exit, poly_key_back, poly_key_repeat, poly_key_next, poly_key_pause, poly_key_left, poly_key_right;
extern unsigned char poly_key_up, poly_key_down, poly_key_u0, poly_key_u1, poly_key_u2, poly_key_u3, poly_key_u4, poly_key_u5, poly_key_u6;
extern unsigned char poly_key_u7, poly_key_u8, poly_key_u9, poly_key_at, poly_key_bar, poly_key_exp, poly_key_pound, poly_key_shift_pause;
extern unsigned char poly_key_keypad_dot;

extern unsigned char key_fake_open_square;
extern unsigned char key_fake_close_square;
extern unsigned char key_fake_open_curly;
extern unsigned char key_fake_close_curly;

extern unsigned char key_fake_close_tilde;
extern unsigned char key_fake_close_open_single_quote;
extern unsigned char key_fake_backslash;
extern unsigned char key_fake_tilde;
extern unsigned char key_fake_close_open_single_quote;
extern unsigned char key_fake_backslash;

extern void poly_set_keyboard_scan_codes(long poly_type);
