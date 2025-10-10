/*
	GRAPHICS.CPP
	------------
*/
#include "graphics.h"

static uint32_t graphics_mixed_colour_table[] =
	{
	// 10 = mix red
	0x00FF0000,			// Red + red = red
	0x00FFFF00,			// Green + red = yellow
	0x00FF00FF,			// Blue + red = magenta
	0x00FFFFFF,			// White + red = red
	// 01 = mix green
	0x00FFFF00,			// Red + green = yellow
	0x0000FF00,			// Green + green = green
	0x0000FFFF,			// Blue + green = cyan
	0x00FFFFFF,			// White + green = white
	// 00 = mix blue
	0x00FF00FF,			// Red + blue = magenta
	0x0000FFFF,			// Green + blue = cyan
	0x000000FF,			// Blue + blue = blue
	0x00FFFFFF,			// White + blue = white
	// 11 = mix nothing
	0x00FF0000,			// Red
	0x0000FF00,			// Green
	0x000000FF,			// Blue
	0x00FFFFFF			// White
	};

static uint32_t graphics_colour_table[] =
	{
	0x00FF0000,			// Red
	0x0000FF00,			// Green
	0x000000FF,			// Blue
	0x00FFFFFF			// White
	};


/*
	GRAPHICS::RENDER_PAGE()
	-----------------------
*/
void graphics::render_page(uint32_t *canvas, unsigned char *memory, unsigned char mix_colour)
{
long x, y;
unsigned char *byte;
uint32_t *pixel_base;
uint32_t colour;

byte = memory;

for (y = 0; y < 204; y++)
	for (x = 0; x < 40; x++)
		{
		if ((*byte & 0x3F) != 0)
			{
			pixel_base = canvas + y * 480 + x * 12;
			colour = graphics_mixed_colour_table[(*byte >> 6) + (mix_colour << 2)];
			if (*byte & 0x20)
				{
				pixel_base[0] = colour;
				pixel_base[1] = colour;
				}
			if (*byte & 0x10)
				{
				pixel_base[2] = colour;
				pixel_base[3] = colour;
				}
			if (*byte & 0x08)
				{
				pixel_base[4] = colour;
				pixel_base[5] = colour;
				}
			if (*byte & 0x04)
				{
				pixel_base[6] = colour;
				pixel_base[7] = colour;
				}
			if (*byte & 0x02)
				{
				pixel_base[8] = colour;
				pixel_base[9] = colour;
				}
			if (*byte & 0x01)
				{
				pixel_base[10] = colour;
				pixel_base[11] = colour;
				}
			}
		byte++;
		}
}

/*
	GRAPHICS::RENDER_MIXED_PAGES()
	------------------------------
*/
void graphics::render_mixed_pages(uint32_t *canvas, unsigned char *screen2, unsigned char *screen4)
{
long x, y;
unsigned char *byte1, *byte2, byte;
uint32_t *pixel_base;
uint32_t colour;

byte1 = screen2;
byte2 = screen4;

for (y = 0; y < 204; y++)
	for (x = 0; x < 40; x++)
		{
		byte = *byte1 & *byte2;
		if ((byte & 0x3F) != 0)
			{
			pixel_base = canvas + y * 480 + x * 12;
			colour = graphics_mixed_colour_table[((*byte1 >> 6) | ((*byte2 & 0xC0) >> 4))];

			if (byte & 0x20)
				{
				pixel_base[0] = colour;
				pixel_base[1] = colour;
				}
			if (byte & 0x10)
				{
				pixel_base[2] = colour;
				pixel_base[3] = colour;
				}
			if (byte & 0x08)
				{
				pixel_base[4] = colour;
				pixel_base[5] = colour;
				}
			if (byte & 0x04)
				{
				pixel_base[6] = colour;
				pixel_base[7] = colour;
				}
			if (byte & 0x02)
				{
				pixel_base[8] = colour;
				pixel_base[9] = colour;
				}
			if (byte & 0x01)
				{
				pixel_base[10] = colour;
				pixel_base[11] = colour;
				}
			}
		byte1++;
		byte2++;
		}
}

/*
	GRAPHICS::PAINT_COMBINED_GRAPHICS_PAGE()
	----------------------------------------
*/
void graphics::render_combined_pages(uint32_t *canvas, unsigned char *screen2, unsigned char *screen4)
{
long x, y;
unsigned char *byte1, *byte2;
uint32_t *pixel_base;
uint32_t colour;

byte1 = screen2;
byte2 = screen4;

for (y = 0; y < 204; y++)
	for (x = 0; x < 40; x++)
		{
		if ((*byte1 & 0x3F) != 0)
			{
			pixel_base = canvas + y * 480 + x * 12;
			colour = graphics_colour_table[(*byte1 >> 6)];
			if (*byte1 & 0x20)
				pixel_base[0] = colour;
			if (*byte1 & 0x10)
				pixel_base[1] = colour;
			if (*byte1 & 0x08)
				pixel_base[2] = colour;
			if (*byte1 & 0x04)
				pixel_base[3] = colour;
			if (*byte1 & 0x02)
				pixel_base[4] = colour;
			if (*byte1 & 0x01)
				pixel_base[5] = colour;
			}
		if ((*byte2 & 0x3F) != 0)
			{
			pixel_base = canvas + y * 480 + x * 12;
			colour = graphics_colour_table[(*byte2 >> 6)];
			if (*byte2 & 0x20)
				pixel_base[6] = colour;
			if (*byte2 & 0x10)
				pixel_base[7] = colour;
			if (*byte2 & 0x08)
				pixel_base[8] = colour;
			if (*byte2 & 0x04)
				pixel_base[9] = colour;
			if (*byte2 & 0x02)
				pixel_base[10] = colour;
			if (*byte2 & 0x01)
				pixel_base[11] = colour;
			}
		byte1++;
		byte2++;
		}
}
