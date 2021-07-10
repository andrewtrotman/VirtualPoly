/*
	SAA5050_FONT.CPP
	----------------
	Copyright (c) 2021 Andrew Trotman
*/
#include "saa5050_font.h"

/*
	Font from the SAA5050 Teletext character generator chip
*/
unsigned char saa5050_font[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 
0x00, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x0A, 0x0A, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
0x09, 0x08, 0x1C, 0x08, 0x08, 0x1F, 0x00, 0x00, 
0x00, 0x0E, 0x15, 0x14, 0x0E, 0x05, 0x15, 0x0E, 
0x00, 0x00, 0x00, 0x18, 0x19, 0x02, 0x04, 0x08, 
0x13, 0x03, 0x00, 0x00, 0x00, 0x08, 0x14, 0x14, 
0x08, 0x15, 0x12, 0x0D, 0x00, 0x00, 0x00, 0x04, 
0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02, 
0x00, 0x00, 0x00, 0x08, 0x04, 0x02, 0x02, 0x02, 
0x04, 0x08, 0x00, 0x00, 0x00, 0x04, 0x15, 0x0E, 
0x04, 0x0E, 0x15, 0x04, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 
0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 
0x00, 0x04, 0x0A, 0x11, 0x11, 0x11, 0x0A, 0x04, 
0x00, 0x00, 0x00, 0x04, 0x0C, 0x04, 0x04, 0x04, 
0x04, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x01, 
0x06, 0x08, 0x10, 0x1F, 0x00, 0x00, 0x00, 0x1F, 
0x01, 0x02, 0x06, 0x01, 0x11, 0x0E, 0x00, 0x00, 
0x00, 0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02, 
0x00, 0x00, 0x00, 0x1F, 0x10, 0x1E, 0x01, 0x01, 
0x11, 0x0E, 0x00, 0x00, 0x00, 0x06, 0x08, 0x10, 
0x1E, 0x11, 0x11, 0x0E, 0x00, 0x00, 0x00, 0x1F, 
0x01, 0x02, 0x04, 0x08, 0x08, 0x08, 0x00, 0x00, 
0x00, 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E, 
0x00, 0x00, 0x00, 0x0E, 0x11, 0x11, 0x0F, 0x01, 
0x02, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 
0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x08, 0x00, 0x00, 0x08, 0x08, 0x10, 0x00, 
0x00, 0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x1F, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x02, 
0x01, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00, 0x0E, 
0x11, 0x01, 0x02, 0x04, 0x00, 0x04, 0x00, 0x00, 
0x00, 0x0E, 0x11, 0x17, 0x15, 0x17, 0x10, 0x0E, 
0x00, 0x00, 0x00, 0x04, 0x0A, 0x11, 0x11, 0x1F, 
0x11, 0x11, 0x00, 0x00, 0x00, 0x1E, 0x11, 0x11, 
0x1E, 0x11, 0x11, 0x1E, 0x00, 0x00, 0x00, 0x0E, 
0x11, 0x10, 0x10, 0x10, 0x11, 0x0E, 0x00, 0x00, 
0x00, 0x1E, 0x09, 0x09, 0x09, 0x09, 0x09, 0x1E, 
0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x1E, 0x10, 
0x10, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 
0x1E, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x0E, 
0x11, 0x10, 0x10, 0x13, 0x11, 0x0F, 0x00, 0x00, 
0x00, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11, 
0x00, 0x00, 0x00, 0x0E, 0x04, 0x04, 0x04, 0x04, 
0x04, 0x0E, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 
0x02, 0x02, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x11, 
0x12, 0x14, 0x18, 0x14, 0x12, 0x11, 0x00, 0x00, 
0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0F, 
0x00, 0x00, 0x00, 0x11, 0x1B, 0x15, 0x15, 0x11, 
0x11, 0x11, 0x00, 0x00, 0x00, 0x11, 0x11, 0x19, 
0x15, 0x13, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0E, 
0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00, 0x00, 
0x00, 0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10, 
0x00, 0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x15, 
0x12, 0x0D, 0x00, 0x00, 0x00, 0x1E, 0x11, 0x11, 
0x1E, 0x14, 0x12, 0x11, 0x00, 0x00, 0x00, 0x0E, 
0x11, 0x10, 0x0E, 0x01, 0x11, 0x0E, 0x00, 0x00, 
0x00, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 
0x11, 0x0E, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 
0x0A, 0x0A, 0x04, 0x04, 0x00, 0x00, 0x00, 0x11, 
0x11, 0x11, 0x15, 0x15, 0x15, 0x0A, 0x00, 0x00, 
0x00, 0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11, 
0x00, 0x00, 0x00, 0x11, 0x11, 0x0A, 0x04, 0x04, 
0x04, 0x04, 0x00, 0x00, 0x00, 0x1F, 0x01, 0x02, 
0x04, 0x08, 0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x08, 0x1F, 0x08, 0x04, 0x00, 0x00, 0x00, 
0x00, 0x10, 0x10, 0x10, 0x10, 0x16, 0x01, 0x02, 
0x04, 0x07, 0x00, 0x00, 0x04, 0x02, 0x1F, 0x02, 
0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0E, 
0x15, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x0A, 
0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x01, 0x0F, 
0x11, 0x0F, 0x00, 0x00, 0x00, 0x10, 0x10, 0x1E, 
0x11, 0x11, 0x11, 0x1E, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0F, 0x10, 0x10, 0x10, 0x0F, 0x00, 0x00, 
0x00, 0x01, 0x01, 0x0F, 0x11, 0x11, 0x11, 0x0F, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x1F, 
0x10, 0x0E, 0x00, 0x00, 0x00, 0x02, 0x04, 0x04, 
0x0E, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0F, 0x11, 0x11, 0x11, 0x0F, 0x01, 0x0E, 
0x00, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x11, 0x11, 
0x00, 0x00, 0x00, 0x04, 0x00, 0x0C, 0x04, 0x04, 
0x04, 0x0E, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0C, 
0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x00, 0x08, 
0x08, 0x09, 0x0A, 0x0C, 0x0A, 0x09, 0x00, 0x00, 
0x00, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x15, 0x15, 
0x15, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 
0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1E, 0x11, 0x11, 0x11, 0x1E, 
0x10, 0x10, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x11, 
0x11, 0x0F, 0x01, 0x01, 0x00, 0x00, 0x00, 0x0B, 
0x0C, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0F, 0x10, 0x0E, 0x01, 0x1E, 0x00, 0x00, 
0x00, 0x00, 0x04, 0x0E, 0x04, 0x04, 0x04, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 
0x11, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 
0x11, 0x0A, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x11, 0x11, 0x15, 0x15, 0x0A, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 
0x11, 0x0F, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x1F, 
0x02, 0x04, 0x08, 0x1F, 0x00, 0x00, 0x00, 0x10, 
0x10, 0x10, 0x10, 0x11, 0x03, 0x05, 0x07, 0x01, 
0x00, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
0x00, 0x00, 0x00, 0x18, 0x04, 0x18, 0x04, 0x19, 
0x03, 0x05, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x1F, 
0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 
0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x00, 
0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 
0x38, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x38, 
0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x3F, 0x3F, 
0x3F, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x00, 
0x00, 0x00, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 
0x07, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x3F, 0x3F, 
0x3F, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 
0x00, 0x00, 0x38, 0x38, 0x38, 0x3F, 0x3F, 0x3F, 
0x3F, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x3F, 
0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x3F, 
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 
0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 0x00, 
0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x3F, 0x3F, 
0x3F, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 
0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x38, 
0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 
0x38, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 0x38, 
0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x3F, 0x3F, 
0x3F, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 
0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x38, 
0x38, 0x38, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 
0x07, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x38, 0x38, 0x38, 0x3F, 0x3F, 
0x3F, 0x07, 0x07, 0x07, 0x07, 0x38, 0x38, 0x38, 
0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x38, 
0x38, 0x38, 0x38, 0x38, 0x38, 0x3F, 0x3F, 0x3F, 
0x3F, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 0x3F, 
0x3F, 0x3F, 0x3F, 0x38, 0x38, 0x38, 0x3F, 0x3F, 
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x38, 0x38, 0x38, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
0x07, 0x07, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 
0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x3F, 0x3F, 
0x3F, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 
0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x07, 
0x07, 0x07, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 
0x38, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x38, 
0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 0x3F, 0x3F, 
0x3F, 0x38, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 
0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x3F, 0x3F, 
0x3F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x07, 
0x07, 0x07, 0x38, 0x38, 0x38, 0x3F, 0x3F, 0x3F, 
0x3F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x3F, 
0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 0x3F, 0x3F, 
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 
0x3F, 0x3F, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 0x00, 
0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
0x3F, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 
0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x3F, 
0x3F, 0x3F, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 
0x38, 0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 0x38, 
0x38, 0x38, 0x38, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
0x3F, 0x38, 0x38, 0x38, 0x38, 0x3F, 0x3F, 0x3F, 
0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x3F, 
0x3F, 0x3F, 0x38, 0x38, 0x38, 0x07, 0x07, 0x07, 
0x07, 0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
0x3F, 0x07, 0x07, 0x07, 0x07, 0x3F, 0x3F, 0x3F, 
0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
0x3F, 0x3F, 0x38, 0x38, 0x38, 0x3F, 0x3F, 0x3F, 
0x3F, 0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 0x3F, 
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x1B, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 
0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x18, 
0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x1B, 
0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 
0x00, 0x00, 0x18, 0x18, 0x00, 0x03, 0x03, 0x03, 
0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x03, 
0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x1B, 
0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1B, 0x1B, 0x1B, 0x00, 0x00, 
0x00, 0x00, 0x18, 0x18, 0x00, 0x1B, 0x1B, 0x1B, 
0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x1B, 
0x1B, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x1B, 
0x00, 0x1B, 0x1B, 0x1B, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x18, 0x18, 0x00, 0x03, 0x03, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x1B, 0x1B, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x18, 
0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 
0x00, 0x18, 0x18, 0x00, 0x03, 0x03, 0x00, 0x18, 
0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x1B, 0x1B, 
0x00, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x18, 
0x18, 0x00, 0x18, 0x18, 0x00, 0x03, 0x03, 0x03, 
0x00, 0x18, 0x18, 0x00, 0x03, 0x03, 0x00, 0x03, 
0x03, 0x03, 0x00, 0x18, 0x18, 0x00, 0x1B, 0x1B, 
0x00, 0x03, 0x03, 0x03, 0x00, 0x18, 0x18, 0x00, 
0x00, 0x00, 0x00, 0x1B, 0x1B, 0x1B, 0x00, 0x18, 
0x18, 0x00, 0x18, 0x18, 0x00, 0x1B, 0x1B, 0x1B, 
0x00, 0x18, 0x18, 0x00, 0x03, 0x03, 0x00, 0x1B, 
0x1B, 0x1B, 0x00, 0x18, 0x18, 0x00, 0x1B, 0x1B, 
0x00, 0x1B, 0x1B, 0x1B, 0x00, 0x18, 0x18, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
0x03, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x1B, 0x1B, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x03, 
0x03, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 
0x00, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x18, 
0x18, 0x18, 0x00, 0x03, 0x03, 0x00, 0x1B, 0x1B, 
0x00, 0x18, 0x18, 0x18, 0x00, 0x03, 0x03, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x03, 
0x03, 0x00, 0x18, 0x18, 0x00, 0x03, 0x03, 0x03, 
0x00, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x03, 
0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x1B, 0x1B, 
0x00, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 
0x00, 0x00, 0x00, 0x1B, 0x1B, 0x1B, 0x00, 0x03, 
0x03, 0x00, 0x18, 0x18, 0x00, 0x1B, 0x1B, 0x1B, 
0x00, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x1B, 
0x1B, 0x1B, 0x00, 0x03, 0x03, 0x00, 0x1B, 0x1B, 
0x00, 0x1B, 0x1B, 0x1B, 0x00, 0x03, 0x03, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 
0x1B, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x1B, 0x1B, 0x00, 0x03, 0x03, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1B, 0x1B, 0x00, 0x1B, 0x1B, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x1B, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x1B, 
0x1B, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 
0x00, 0x1B, 0x1B, 0x00, 0x03, 0x03, 0x00, 0x18, 
0x18, 0x18, 0x00, 0x1B, 0x1B, 0x00, 0x1B, 0x1B, 
0x00, 0x18, 0x18, 0x18, 0x00, 0x1B, 0x1B, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x1B, 
0x1B, 0x00, 0x18, 0x18, 0x00, 0x03, 0x03, 0x03, 
0x00, 0x1B, 0x1B, 0x00, 0x03, 0x03, 0x00, 0x03, 
0x03, 0x03, 0x00, 0x1B, 0x1B, 0x00, 0x1B, 0x1B, 
0x00, 0x03, 0x03, 0x03, 0x00, 0x1B, 0x1B, 0x00, 
0x00, 0x00, 0x00, 0x1B, 0x1B, 0x1B, 0x00, 0x1B, 
0x1B, 0x00, 0x18, 0x18, 0x00, 0x1B, 0x1B, 0x1B, 
0x00, 0x1B, 0x1B, 0x00, 0x03, 0x03, 0x00, 0x1B, 
0x1B, 0x1B, 0x00, 0x1B, 0x1B, 0x00, 0x1B, 0x1B, 
0x00, 0x1B, 0x1B, 0x1B, 0x00, 0x1B, 0x1B, 0x00, 
};
