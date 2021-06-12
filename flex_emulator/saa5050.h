/*
	SAA5050.H
	---------
*/
#ifndef SAA5050_H_
#define SAA5050_H_

/*
	class SAA5050
	-------------
*/
class saa5050
{
protected:
	unsigned char *font;
	unsigned char *screen_memory;

	unsigned char text_colour;
	unsigned char background_text_colour;
	long glyph_base;
	long text_graphics_mode;
	long text_double_height;
	long conceal;
	long flashing;

protected:
	long print_char(unsigned char *canvas, long pos_x, long pos_y, unsigned char ch, long text_flash_state);

public:
	saa5050(unsigned char *memory);
	virtual ~saa5050() {}

	void paint_text_page(unsigned char *canvas, long text_flash_state);
} ;

#endif /* SAA5050_H_ */
