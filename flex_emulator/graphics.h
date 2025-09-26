/*
	GRAPHICS.H
	----------
*/
#pragma once
#include <stdint.h>

/*
	class GRAPHICS
	--------------
*/
class graphics
{
public:
	static void render_page(uint32_t *canvas, unsigned char *memory, unsigned char mix_colour);
	static void render_mixed_pages(uint32_t *canvas, unsigned char *screen2, unsigned char *screen4);
	static void render_combined_pages(uint32_t *canvas, unsigned char *screen2, unsigned char *screen4);
} ;
