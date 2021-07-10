/*
	SAA5050.CPP
	-----------
	Copyright (c) 2021 Andrew Trotman
*/
#include "saa5050.h"
#include "saa5050_font.h"
#include "saa5055_font.h"

#define FALSE 0
#define TRUE (!FALSE)

/*
	SAA5050::SAA5050()
	------------------
*/
saa5050::saa5050(unsigned char *memory)
{
font = saa5055_font;
//font = saa5050_font;
this->screen_memory = memory;
text_colour = background_text_colour = 0;
glyph_base = text_graphics_mode = text_double_height = conceal = 0;
}

/*
	SAA5050::PRINT_CHAR()
	---------------------
*/
long saa5050::print_char(unsigned char *canvas, long pos_x, long pos_y, unsigned char ch, long text_flash_state)
{
long true_glyph_base;
long from, x, y, pos;
unsigned char *into;
unsigned char on, off, background_colour;

if (ch == 0x00)		// 0x00 isn't used (and isn't special either)
	ch = ' ';

if (ch == 25)
	{
	text_graphics_mode = 0;		// continious graphics
	if (glyph_base != 0)
		glyph_base = 128 - 32;
	}
else if (ch == 26)
	{
	text_graphics_mode = 1;		// seperate graphics
	if (glyph_base != 0)
		glyph_base = 128 + 32;
	}
else if (ch >= 17 && ch <= 23)		// coloured graphics on
	{
	ch -= 16;		// convert into a colour selector
	if (text_graphics_mode == 0)
		glyph_base = 128 - 32;		// continious graphics
	else
		glyph_base = 128 + 32;		// seperate graphics
	}
else if (ch == 28)					// no background
	background_text_colour = 0;
else if (ch == 29)					// set background to current colour
	background_text_colour = text_colour;
else if (ch == 24)					// conceal to end of line
	conceal = 1;
else if (ch == 13)					// double height
	text_double_height = TRUE;
else if (ch == 12)					// normal height
	text_double_height = FALSE;
else if (ch == 8)
	flashing = TRUE;					// flashing on
else if (ch == 9)
	flashing = FALSE;					// flashing off
else if (ch <= 7)					// coloured text
	glyph_base = 0;

background_colour = background_text_colour;

if (ch & 0x80)		// high bit set is the indicator of inverse video
	{
	ch &= 0x7F;
	on = (~text_colour) & 0x07;
	off = (~background_colour) & 0x07;
	}
else
	{
	on = text_colour;
	off = background_colour;
	}

/*
	Shift out of coloured graphics mode into coloured text mode
*/
if (ch <= 7)
	text_colour = ch;

if (ch < ' ')
	ch = ' ';
if (conceal)
	ch = ' ';

true_glyph_base = glyph_base;
if (glyph_base != 0)
	{
	if (ch >= 96)
		ch -= 32;
	else if (ch >= 64 && ch <= 95)
		true_glyph_base = 0;			// use regular character for '@'->'#'
	}

if (flashing && text_flash_state)
	ch = ' ';

from = (long)(ch - ' ' + true_glyph_base) * 10;		// 10 bytes per character

if (text_double_height)
	for (y = 0; y < 20; y++)
		{
		pos = font[from + (y >> 1)];
		into = canvas + (pos_x * 12 + pos_y * 10 * 480) + y * 480;

		for (x = 2; x < 8; x++)
			if (pos & (0x80 >> x))
				if (on != 0)
					{
					*into++ = on;
					*into++ = on;
					}
				else
					{
					into++;
					into++;
					}
			else
				if (off != 0)
					{
					*into++ = off;
					*into++ = off;
					}
				else
					{
					into++;
					into++;
					}
		}
else
	for (y = 0; y < 10; y++)
		{
		pos = font[from + y];
		into = canvas + (pos_x * 12 + pos_y * 10 * 480) + y * 480;

		for (x = 2; x < 8; x++)
			if (pos & (0x80 >> x))
				if (on != 0)
					{
					*into++ = on;
					*into++ = on;
					}
				else
					{
					into++;
					into++;
					}
			else
				if (off != 0)
					{
					*into++ = off;
					*into++ = off;
					}
				else
					{
					into++;
					into++;
					}
		}

return text_double_height;
}

/*
	SAA5050::PAINT_TEXT_PAGE()
	--------------------------
*/
void saa5050::paint_text_page(unsigned char *canvas, long text_flash_state)
{
long line, column, double_height;
unsigned char ch;

for (line = 0; line < 24; line++)
	{
	text_colour = 7;
	background_text_colour = 0;
	glyph_base = 0;
	text_graphics_mode = 0;
	text_double_height = 0;
	double_height = 0;
	conceal = 0;
	flashing = 0;

	for (column = 0; column < 40; column++)			// are we about to enter double hight mode?
		{
		ch = *(screen_memory + column + line * 40);
		if (ch == 13)
			text_double_height = TRUE;
		}
	for (column = 0; column < 40; column++)			// now print the text
		{
		ch = *(screen_memory + column + line * 40);
		double_height += print_char(canvas, column, line, ch, text_flash_state);
		}
	if (double_height)
		line++;
	}
}
