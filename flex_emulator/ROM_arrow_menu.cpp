/*
	ROM_ARROW_MENU.CPP
	------------------
	Copyright (c) 2021 Andrew Trotman
	ROM contained herein is (probably) Copyright (c) Mark Wilson
*/
#include "ROM_arrow_menu.h"

/*
	ROM_ARROW_MENU[]
	----------------
	ROM from the Arrow Micro by Mark Wilson
	According to Mark, this sits at $F000-FFFF and is the "Start menu, system function library, and vectors"
*/
const unsigned char ROM_arrow_menu[] =
{
0x30, 0x8C, 0x7D, 0xAD, 0x9F, 0xFF, 0xE8, 0xAD, 0x9F, 0xFF, 0xE0, 0xAD, 0x9F, 0xFF, 0xE2, 0x1F,
0x89, 0xAD, 0x9F, 0xFF, 0xE0, 0x81, 0x0D, 0x26, 0xE7, 0x1F, 0x98, 0x8D, 0x51, 0x81, 0x31, 0x2D,
0xDF, 0x26, 0x05, 0x8E, 0x90, 0x00, 0x20, 0x41, 0x81, 0x34, 0x26, 0x05, 0x8E, 0xA0, 0x00, 0x20,
0x38, 0x8D, 0x44, 0x81, 0x33, 0x26, 0x05, 0x8E, 0x80, 0x00, 0x20, 0x2D, 0x81, 0x32, 0x26, 0x05,
0x8E, 0xA0, 0x00, 0x20, 0x24, 0x81, 0x35, 0x26, 0x05, 0x8E, 0xC0, 0x00, 0x20, 0x1B, 0x81, 0x36,
0x26, 0x05, 0x8E, 0xD0, 0x00, 0x20, 0x12, 0x81, 0x37, 0x2E, 0xA5, 0x26, 0xA3, 0x8E, 0x00, 0x00,
0x6F, 0x80, 0x8C, 0x08, 0x00, 0x2D, 0xF9, 0x20, 0x97, 0xBF, 0xEF, 0xC4, 0x6E, 0x84, 0xF6, 0xE0,
0x00, 0xC4, 0xE7, 0xF7, 0xE0, 0x00, 0x39, 0xF6, 0xE0, 0x00, 0xC4, 0xE7, 0xCA, 0x08, 0x20, 0xF3,
0x0C, 0x0D, 0x0A, 0x0A, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3E, 0x3E, 0x3E, 0x2D,
0x2D, 0x3E, 0x20, 0x41, 0x52, 0x52, 0x4F, 0x57, 0x20, 0x43, 0x6F, 0x6D, 0x70, 0x75, 0x74, 0x65,
0x72, 0x20, 0x2D, 0x20, 0x4D, 0x65, 0x6E, 0x75, 0x20, 0x3C, 0x2D, 0x3C, 0x3C, 0x3C, 0x20, 0x20,
0x20, 0x20, 0x20, 0x31, 0x36, 0x2F, 0x30, 0x37, 0x2F, 0x38, 0x35, 0x0D, 0x0A, 0x0A, 0x0A, 0x31,
0x20, 0x2D, 0x20, 0x42, 0x41, 0x53, 0x49, 0x43, 0x0D, 0x0A, 0x32, 0x20, 0x2D, 0x20, 0x50, 0x41,
0x53, 0x43, 0x41, 0x4C, 0x0D, 0x0A, 0x33, 0x20, 0x2D, 0x20, 0x46, 0x4F, 0x52, 0x54, 0x48, 0x0D,
0x0A, 0x34, 0x20, 0x2D, 0x20, 0x41, 0x73, 0x73, 0x65, 0x6D, 0x62, 0x6C, 0x65, 0x72, 0x20, 0x2F,
0x20, 0x4D, 0x6F, 0x6E, 0x69, 0x74, 0x6F, 0x72, 0x0D, 0x0A, 0x35, 0x20, 0x2D, 0x20, 0x52, 0x4F,
0x4D, 0x20, 0x61, 0x74, 0x20, 0x24, 0x43, 0x30, 0x30, 0x30, 0x0D, 0x0A, 0x36, 0x20, 0x2D, 0x20,
0x52, 0x4F, 0x4D, 0x20, 0x61, 0x74, 0x20, 0x24, 0x44, 0x30, 0x30, 0x30, 0x0D, 0x0A, 0x37, 0x20,
0x2D, 0x20, 0x43, 0x6C, 0x65, 0x61, 0x72, 0x20, 0x6D, 0x65, 0x6D, 0x6F, 0x72, 0x79, 0x0D, 0x0A,
0x0A, 0x0A, 0x45, 0x6E, 0x74, 0x65, 0x72, 0x20, 0x79, 0x6F, 0x75, 0x72, 0x20, 0x63, 0x68, 0x6F,
0x69, 0x63, 0x65, 0x20, 0x3A, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x34, 0x36, 0x8E, 0x01, 0x01, 0x4F, 0x5F, 0x34, 0x16, 0xA6, 0x45, 0xE6, 0x43, 0xA0, 0x49, 0x24,
0x03, 0x40, 0x60, 0x62, 0xE0, 0x47, 0x24, 0x03, 0x50, 0x60, 0x63, 0x34, 0x04, 0xA1, 0xE0, 0x34,
0x06, 0x24, 0x0A, 0x1E, 0x89, 0xED, 0xE4, 0xA6, 0x65, 0xA7, 0x63, 0x20, 0x04, 0xA6, 0x64, 0xA7,
0x62, 0xA6, 0xE4, 0xA0, 0x61, 0x34, 0x02, 0xE6, 0x61, 0x54, 0x4F, 0xE0, 0xE4, 0x82, 0x00, 0x34,
0x06, 0xA6, 0x49, 0xE6, 0x47, 0x8D, 0x2E, 0x6D, 0xE4, 0x2A, 0x0E, 0xAB, 0x65, 0xEB, 0x66, 0x34,
0x06, 0xEC, 0x62, 0xEB, 0x66, 0x89, 0x00, 0x20, 0x0C, 0xAB, 0x67, 0xEB, 0x68, 0x34, 0x06, 0xEC,
0x62, 0xE0, 0x64, 0x82, 0x00, 0xED, 0x62, 0x35, 0x06, 0x6A, 0x63, 0x26, 0xD8, 0x8D, 0x06, 0x33,
0x4A, 0x32, 0x69, 0x35, 0xB6, 0x1F, 0x02, 0x34, 0x46, 0x1F, 0x89, 0x1F, 0x01, 0xEC, 0xC4, 0x36,
0x36, 0x8D, 0x02, 0x35, 0xC6, 0x8D, 0x37, 0xE6, 0x41, 0x5A, 0x27, 0x0B, 0x5A, 0x27, 0x15, 0x5A,
0x27, 0x1A, 0x5A, 0x27, 0x1D, 0x20, 0x0A, 0x4D, 0x27, 0x07, 0xAA, 0x84, 0xA7, 0x84, 0xBF, 0xEF,
0x80, 0x33, 0x46, 0x39, 0x4D, 0x27, 0xFA, 0x43, 0xA4, 0x84, 0x20, 0xF0, 0x1F, 0x89, 0xE8, 0x84,
0xE7, 0x84, 0x5F, 0xA4, 0x84, 0x27, 0x01, 0x5C, 0x33, 0x44, 0x4F, 0xED, 0xC4, 0x39, 0xA6, 0x45,
0xC6, 0x56, 0x3D, 0x34, 0x04, 0xC6, 0x40, 0x3D, 0x36, 0x06, 0xFC, 0xEF, 0x84, 0xC4, 0xC0, 0xA3,
0xC1, 0x1F, 0x01, 0x35, 0x04, 0xA6, 0x43, 0x2B, 0x29, 0x47, 0x30, 0x86, 0xBC, 0xEF, 0x82, 0x25,
0x21, 0x86, 0x03, 0x3D, 0xC6, 0x10, 0x4A, 0x2D, 0x04, 0x57, 0x57, 0x20, 0xF9, 0xA6, 0x43, 0x47,
0x24, 0x01, 0x58, 0xA6, 0x84, 0x84, 0xC0, 0x81, 0x80, 0x27, 0x04, 0x86, 0x80, 0xA7, 0x84, 0x1F,
0x98, 0x39, 0x4F, 0x39, 0x34, 0x3A, 0x86, 0xEF, 0x1F, 0x8B, 0x37, 0x16, 0x9F, 0x97, 0xDD, 0x99,
0x8E, 0x40, 0x00, 0x8D, 0x5B, 0x8D, 0x5B, 0x30, 0x1F, 0x26, 0xF8, 0x86, 0xB3, 0x8D, 0x24, 0x0F,
0x9B, 0x8E, 0xEF, 0x97, 0xC6, 0x04, 0xA6, 0x80, 0x8D, 0x19, 0x5A, 0x26, 0xF9, 0x96, 0x9B, 0x8D,
0x12, 0x0F, 0x9B, 0x9E, 0x97, 0xA6, 0x80, 0x8D, 0x0A, 0x9C, 0x99, 0x23, 0xF8, 0x96, 0x9B, 0x8D,
0x02, 0x35, 0xBA, 0x34, 0x04, 0x17, 0x04, 0x68, 0x1F, 0x89, 0xDB, 0x9B, 0xD7, 0x9B, 0x8D, 0x20,
0x8D, 0x1E, 0xC6, 0x08, 0xD7, 0x9C, 0x8D, 0x18, 0x85, 0x01, 0x27, 0x04, 0x8D, 0x12, 0x20, 0x02,
0x8D, 0x10, 0x44, 0x0A, 0x9C, 0x2E, 0xEF, 0x8D, 0x07, 0x8D, 0x07, 0x17, 0x04, 0x42, 0x35, 0x84,
0xC6, 0x08, 0x10, 0x8E, 0xE0, 0x00, 0xEA, 0x21, 0xE7, 0x21, 0xD6, 0x9E, 0x5A, 0x26, 0xFD, 0xE6,
0x21, 0xC4, 0xF7, 0xE7, 0x21, 0xD6, 0x9E, 0x12, 0x20, 0x00, 0x5A, 0x26, 0xFA, 0x39, 0x34, 0x38,
0x86, 0xEF, 0x1F, 0x8B, 0xE6, 0xC0, 0xEA, 0xC0, 0xD7, 0xA0, 0x17, 0x00, 0x89, 0x17, 0x02, 0xD9,
0x1F, 0x12, 0xCC, 0xF2, 0xF3, 0xED, 0xA4, 0x8D, 0x5E, 0x81, 0xB3, 0x26, 0xFA, 0x0F, 0x9B, 0x8E,
0xEF, 0x97, 0xC6, 0x04, 0x8D, 0x51, 0xA7, 0x80, 0x9B, 0x9B, 0x97, 0x9B, 0x5A, 0x26, 0xF5, 0x8D,
0x46, 0x91, 0x9B, 0x26, 0x3C, 0x0F, 0x9B, 0x9E, 0x95, 0x27, 0x0A, 0xDC, 0x99, 0x93, 0x97, 0xD3,
0x95, 0x9F, 0x97, 0xDD, 0x99, 0x9E, 0x97, 0x8D, 0x2E, 0x0D, 0xA0, 0x27, 0x06, 0xA1, 0x80, 0x26,
0x20, 0x20, 0x02, 0xA7, 0x80, 0x1F, 0x89, 0x9B, 0x9B, 0x97, 0x9B, 0xC1, 0x20, 0x26, 0x06, 0xA6,
0xA4, 0x88, 0x02, 0xA7, 0xA4, 0x9C, 0x99, 0x23, 0xDE, 0x8D, 0x0C, 0xC6, 0x01, 0x91, 0x9B, 0x27,
0x01, 0x5F, 0x4F, 0x36, 0x06, 0x35, 0xB8, 0x17, 0x03, 0xA6, 0x86, 0x08, 0x97, 0x9C, 0x8D, 0x4B,
0x2A, 0xFC, 0x0A, 0x9C, 0x2D, 0x0D, 0x44, 0x34, 0x02, 0x8D, 0x40, 0x35, 0x02, 0x2A, 0xF3, 0x8A,
0x80, 0x20, 0xEF, 0x16, 0x03, 0x8A, 0x10, 0x8E, 0xEF, 0x99, 0x5F, 0x86, 0x50, 0x97, 0x9F, 0x6F,
0xA4, 0x6F, 0x21, 0x8D, 0x26, 0x2A, 0xF3, 0x5A, 0x26, 0xF9, 0x8D, 0x1F, 0x9B, 0x9F, 0xAB, 0x21,
0xA7, 0x21, 0x24, 0x02, 0x6C, 0xA4, 0x5C, 0xC1, 0x10, 0x2D, 0xEF, 0x64, 0xA4, 0x66, 0x21, 0x54,
0x24, 0xF9, 0xA6, 0x21, 0x64, 0x21, 0xAB, 0x21, 0x97, 0x9F, 0x39, 0x34, 0x10, 0x8E, 0xE0, 0x00,
0x6D, 0x84, 0x6D, 0x01, 0x2A, 0xFC, 0x6D, 0x84, 0x4F, 0x4C, 0x91, 0x9F, 0x27, 0x04, 0x6D, 0x01,
0x2A, 0xF7, 0x90, 0x9F, 0x35, 0x90, 0x34, 0x1C, 0xC6, 0xEF, 0x1F, 0x9B, 0x17, 0x01, 0xFA, 0xE6,
0x84, 0x34, 0x06, 0x96, 0x87, 0xA7, 0x84, 0x8D, 0x3F, 0x4D, 0x26, 0x16, 0x0F, 0x91, 0x6A, 0xE4,
0x26, 0xF5, 0xE6, 0x61, 0x27, 0xF1, 0x96, 0x87, 0xA1, 0x84, 0x26, 0x02, 0x1F, 0x98, 0xA7, 0x84,
0x20, 0xE5, 0xC6, 0xFF, 0x91, 0x91, 0x26, 0x09, 0x5A, 0x26, 0xFD, 0x0A, 0x92, 0x26, 0xD8, 0xC6,
0x19, 0xD7, 0x92, 0x97, 0x91, 0x32, 0x61, 0x35, 0x04, 0xE7, 0x84, 0x81, 0x0D, 0x26, 0x07, 0x8D,
0x07, 0x4D, 0x26, 0xFB, 0x86, 0x0D, 0x35, 0x9C, 0x34, 0x34, 0x8E, 0xE0, 0x08, 0x7D, 0xEF, 0x93,
0x2B, 0x10, 0xA6, 0x84, 0x2B, 0x01, 0x4F, 0x84, 0x7F, 0x27, 0x18, 0xB1, 0xEF, 0x91, 0x27, 0xF2,
0x20, 0x11, 0x6F, 0x04, 0xA6, 0x84, 0x43, 0x27, 0x0A, 0x8D, 0x0A, 0x34, 0x02, 0x8D, 0x06, 0xA1,
0xE0, 0x26, 0xF6, 0x35, 0xB4, 0x86, 0xFF, 0x44, 0xA7, 0x04, 0xE6, 0x84, 0xCA, 0x80, 0x53, 0x26,
0x05, 0x4D, 0x26, 0xF3, 0x20, 0x24, 0x5F, 0x5C, 0x44, 0x25, 0xFC, 0x5A, 0x86, 0x07, 0x3D, 0x5A,
0xA6, 0x84, 0x5C, 0x44, 0x25, 0xFC, 0x31, 0x8C, 0x67, 0xE6, 0xA5, 0x7D, 0xEF, 0x94, 0x2A, 0x0A,
0xC1, 0x41, 0x2D, 0x06, 0xC1, 0x5A, 0x2E, 0x02, 0xCA, 0x20, 0x8D, 0x03, 0x1F, 0x98, 0x39, 0x86,
0xFF, 0x4D, 0x27, 0x4B, 0x44, 0xA7, 0x04, 0x6D, 0x84, 0x2B, 0xF6, 0x81, 0x03, 0x26, 0x03, 0xC6,
0x03, 0x39, 0x81, 0x1F, 0x26, 0x08, 0x73, 0xEF, 0x94, 0x6D, 0x84, 0x2A, 0xFC, 0x39, 0x81, 0x0F,
0x26, 0x07, 0xC1, 0x40, 0x2F, 0x29, 0xC4, 0x1F, 0x39, 0x81, 0x7F, 0x26, 0x0B, 0xC1, 0x31, 0x2D,
0x11, 0xC1, 0x3F, 0x2E, 0x18, 0xC4, 0x2F, 0x39, 0x81, 0x3F, 0x26, 0x13, 0x5D, 0x27, 0x10, 0xCA,
0xC0, 0x39, 0xC1, 0x2F, 0x2E, 0x09, 0xC1, 0x2C, 0x2D, 0x05, 0xCA, 0x10, 0x39, 0xC8, 0x20, 0x39,
0x1B, 0x32, 0x34, 0x36, 0x38, 0x30, 0x2D, 0x31, 0x33, 0x35, 0x37, 0x39, 0x3A, 0x5E, 0x09, 0x57,
0x52, 0x59, 0x49, 0x50, 0x5D, 0x51, 0x45, 0x54, 0x55, 0x4F, 0x5B, 0x0A, 0x41, 0x44, 0x47, 0x4A,
0x4C, 0x40, 0x7F, 0x00, 0x53, 0x46, 0x48, 0x4B, 0x3B, 0x5C, 0x5A, 0x43, 0x42, 0x4D, 0x2E, 0x20,
0x0D, 0x0B, 0x58, 0x56, 0x4E, 0x2C, 0x2F, 0x08, 0x34, 0x38, 0xC6, 0xEF, 0x1F, 0x9B, 0x17, 0x00,
0xB8, 0x9F, 0x88, 0xBD, 0xFA, 0x76, 0x81, 0x20, 0x2D, 0x10, 0x81, 0x7F, 0x27, 0x0C, 0x0D, 0x86,
0x27, 0x08, 0x1F, 0x89, 0x86, 0x01, 0x8D, 0x24, 0x1F, 0x98, 0x8D, 0x20, 0x81, 0x03, 0x10, 0x27,
0x03, 0x08, 0x81, 0x0D, 0x26, 0xDD, 0x9E, 0x88, 0x1F, 0x12, 0x10, 0x9C, 0x84, 0x24, 0x06, 0x31,
0x21, 0x6D, 0xA4, 0x26, 0xF5, 0x36, 0x30, 0x10, 0x9F, 0x80, 0x35, 0xB8, 0x34, 0x3E, 0xC6, 0xEF,
0x1F, 0x9B, 0xB4, 0xFF, 0xF0, 0x0D, 0x8A, 0x27, 0x03, 0x17, 0x01, 0xD7, 0x1F, 0x89, 0x8D, 0x69,
0x8D, 0x77, 0x25, 0x02, 0xE7, 0x80, 0x9F, 0x80, 0x86, 0x9F, 0xB7, 0xE0, 0x0C, 0x7D, 0xE0, 0x08,
0x2B, 0x0A, 0xB7, 0xE0, 0x0C, 0xF6, 0xE0, 0x08, 0xC4, 0x20, 0x26, 0xF6, 0x35, 0xBE, 0x34, 0x74,
0x9E, 0x82, 0xA6, 0x88, 0x40, 0xA7, 0x80, 0xAC, 0x63, 0x26, 0xF7, 0xDC, 0x88, 0x83, 0x00, 0x40,
0xDD, 0x88, 0xCE, 0x00, 0x00, 0xC6, 0x20, 0xEF, 0x81, 0x5A, 0x26, 0xFB, 0x35, 0xF4, 0x34, 0x74,
0x9E, 0x84, 0x30, 0x88, 0xC1, 0xA6, 0x82, 0xA7, 0x88, 0x40, 0xAC, 0x63, 0x26, 0xF7, 0xDC, 0x88,
0x20, 0xDE, 0x10, 0x9E, 0x82, 0x8D, 0xE7, 0x30, 0x88, 0x40, 0x20, 0x11, 0x10, 0x9E, 0x84, 0x31,
0xA8, 0xC1, 0x8D, 0xBA, 0x30, 0x88, 0xC0, 0x20, 0x04, 0x9E, 0x80, 0x34, 0x20, 0x9C, 0x82, 0x25,
0xE1, 0x9C, 0x84, 0x22, 0xE7, 0x9F, 0x80, 0x35, 0xA0, 0xC1, 0x7F, 0x27, 0x32, 0xC1, 0x1A, 0x22,
0x10, 0x31, 0x8C, 0x10, 0x31, 0xA5, 0xE6, 0xA4, 0x31, 0x8D, 0x00, 0x8B, 0xAD, 0xA5, 0x1A, 0x01,
0x39, 0x1C, 0xFE, 0x39, 0x00, 0x0B, 0x57, 0x00, 0xA9, 0xAE, 0x00, 0x00, 0xA7, 0xA9, 0xB1, 0xAE,
0xCB, 0xD8, 0x00, 0x00, 0x08, 0x01, 0xDF, 0xA7, 0xF2, 0xEA, 0x05, 0x00, 0xB1, 0x00, 0x00, 0x1F,
0x12, 0xA6, 0xA0, 0xA7, 0x3E, 0x27, 0x07, 0x10, 0x9C, 0x84, 0x23, 0xF5, 0x6F, 0x3F, 0x30, 0x1E,
0x30, 0x01, 0x1A, 0x01, 0x39, 0x30, 0x88, 0x80, 0x30, 0x88, 0x40, 0x1F, 0x10, 0xC4, 0xC0, 0x1F,
0x02, 0x31, 0x3F, 0x10, 0x9C, 0x82, 0x25, 0x04, 0x6D, 0xA4, 0x26, 0xF5, 0x31, 0x21, 0x10, 0x9F,
0x88, 0x39, 0x9E, 0x84, 0x30, 0x01, 0x4F, 0xA7, 0x82, 0x9C, 0x82, 0x26, 0xFA, 0x20, 0xDC, 0x1F,
0x10, 0xC4, 0xC0, 0x1F, 0x01, 0x39, 0xCC, 0xE8, 0x00, 0xDD, 0x82, 0xCC, 0xED, 0xFF, 0xDD, 0x84,
0x39, 0x30, 0x1F, 0x9F, 0x84, 0x9F, 0x88, 0x20, 0x02, 0x9F, 0x82, 0xDC, 0x82, 0xC3, 0x00, 0x80,
0x10, 0x93, 0x84, 0x25, 0x02, 0xDD, 0x84, 0x39, 0x9E, 0x82, 0x20, 0xAF, 0x03, 0x86, 0x39, 0x03,
0x8A, 0x39, 0xC6, 0x20, 0x34, 0x16, 0x9C, 0x84, 0x27, 0x32, 0x6D, 0x80, 0x26, 0xF8, 0x1F, 0x10,
0xC5, 0x3F, 0x26, 0x02, 0x8D, 0x3A, 0x30, 0x1F, 0xA6, 0x1F, 0xA7, 0x84, 0xAC, 0x62, 0x26, 0xF6,
0x35, 0x16, 0xE7, 0x84, 0x1F, 0x12, 0x10, 0x9C, 0x82, 0x23, 0x10, 0x1E, 0x02, 0xC5, 0x3F, 0x27,
0x0A, 0x1E, 0x02, 0x6D, 0xA2, 0x26, 0x04, 0xE7, 0xA4, 0x20, 0xEB, 0x39, 0x31, 0x88, 0xC1, 0x17,
0xFE, 0xBC, 0xEC, 0x62, 0x83, 0x00, 0x40, 0xED, 0x62, 0x30, 0x88, 0xC0, 0x20, 0xCA, 0xDC, 0x80,
0xC4, 0xC0, 0x1F, 0x02, 0xDC, 0x84, 0xC4, 0xC0, 0x34, 0x06, 0x10, 0xAC, 0xE1, 0x10, 0x26, 0xFE,
0xBD, 0x34, 0x74, 0x16, 0xFE, 0xAA, 0x7A, 0xEF, 0x8D, 0x2E, 0x12, 0x34, 0x12, 0x86, 0x32, 0x8E,
0xEF, 0x91, 0xA7, 0x1C, 0x6C, 0x82, 0x27, 0xFC, 0xBD, 0xEF, 0xC1, 0x35, 0x12, 0x7E, 0xEF, 0xB2,
0x34, 0x06, 0xFC, 0xEF, 0xC0, 0x1E, 0x89, 0xFD, 0xEF, 0xC0, 0x35, 0x86, 0x34, 0x10, 0xEC, 0xC4,
0xB6, 0xE0, 0x00, 0x84, 0xFE, 0xB7, 0xE0, 0x00, 0x8E, 0x00, 0x00, 0xF5, 0xE0, 0x00, 0x27, 0x04,
0x30, 0x01, 0x20, 0xF7, 0x8A, 0x01, 0xB7, 0xE0, 0x00, 0xAF, 0xC4, 0x30, 0x01, 0x30, 0x1F, 0x26,
0xFC, 0x35, 0x90, 0x34, 0x16, 0x1C, 0xFE, 0xC6, 0x0A, 0x34, 0x04, 0x46, 0xF6, 0xE0, 0x00, 0x4D,
0x2A, 0x04, 0xCA, 0x80, 0x20, 0x02, 0xC4, 0x7F, 0xF7, 0xE0, 0x00, 0xBE, 0xEF, 0xA4, 0x30, 0x1F,
0x26, 0xFC, 0x46, 0x8A, 0x40, 0x6A, 0xE4, 0x26, 0xE3, 0x35, 0x04, 0x35, 0x96, 0x34, 0x54, 0xFC,
0xEF, 0xA4, 0x44, 0x56, 0x1F, 0x01, 0xB6, 0xE0, 0x00, 0x84, 0x40, 0x26, 0xF2, 0x30, 0x1F, 0x26,
0xFC, 0xB6, 0xE0, 0x00, 0x84, 0x40, 0x26, 0xE7, 0xC6, 0x09, 0x34, 0x04, 0xBE, 0xEF, 0xA4, 0x30,
0x1F, 0x26, 0xFC, 0xF6, 0xE0, 0x00, 0x59, 0x59, 0x46, 0x6A, 0xE4, 0x26, 0xEF, 0x49, 0x32, 0x61,
0x35, 0xD4, 0xB6, 0xE0, 0x00, 0x84, 0x40, 0x88, 0x40, 0x39, 0xC6, 0xA5, 0xF1, 0xEF, 0xC8, 0x27,
0x23, 0xF7, 0xEF, 0xC8, 0x8E, 0x00, 0x00, 0x96, 0x00, 0x34, 0x02, 0x30, 0x01, 0xA6, 0x01, 0xE7,
0x01, 0x0F, 0x00, 0xE1, 0x01, 0x34, 0x01, 0xA7, 0x01, 0x35, 0x01, 0x27, 0xEE, 0xBF, 0xEF, 0xC6,
0x35, 0x02, 0x97, 0x00, 0xBE, 0xEF, 0xC6, 0x1F, 0x10, 0x39, 0x8E, 0xEF, 0x80, 0xCC, 0xE8, 0x00,
0xED, 0x81, 0xED, 0x81, 0xCC, 0xED, 0xFF, 0xED, 0x81, 0xC6, 0xFF, 0x4F, 0xA7, 0x04, 0x43, 0xED,
0x81, 0x86, 0x0C, 0x17, 0xFD, 0x66, 0xA7, 0x03, 0xA7, 0x0C, 0x6F, 0x0B, 0x7D, 0xE0, 0x08, 0x2A,
0x02, 0x6A, 0x0B, 0x6F, 0x0D, 0x6F, 0x0E, 0x8E, 0xE0, 0x00, 0x6F, 0x01, 0xCC, 0xB9, 0x34, 0xED,
0x84, 0x84, 0xC7, 0xAA, 0x84, 0xA7, 0x84, 0x86, 0x18, 0xB7, 0xEF, 0x9E, 0xCC, 0x02, 0x71, 0xFD,
0xEF, 0xA4, 0x8D, 0x86, 0x86, 0x3B, 0xB7, 0xEF, 0xB2, 0xB7, 0xEF, 0xBA, 0xFC, 0xFF, 0xFE, 0xFD,
0xEF, 0xC4, 0xCC, 0x39, 0x7E, 0xFD, 0xEF, 0xC0, 0xCC, 0xFE, 0xEA, 0xFD, 0xEF, 0xC2, 0x1C, 0xBF,
0x39, 0xA6, 0x80, 0x17, 0xFD, 0x16, 0x4D, 0x26, 0xF8, 0x39, 0x86, 0xFB, 0xB7, 0xE0, 0x0C, 0x7D,
0xE0, 0x08, 0x2B, 0x0A, 0x10, 0xCE, 0xEF, 0x00, 0x1C, 0xBF, 0x6E, 0x9F, 0xEF, 0xC4, 0x39, 0x10,
0xCE, 0xEF, 0x00, 0xAD, 0x9F, 0xFF, 0xEE, 0x7E, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFE, 0x0D, 0xFD, 0xE3, 0xFE, 0x42, 0xF8, 0x85, 0xFD, 0xBC, 0xF8, 0x00, 0xFE, 0x4A, 0xFD, 0xB0,
0xFA, 0x76, 0xFB, 0xFC, 0xFA, 0xC8, 0xFB, 0xB8, 0xFE, 0xE1, 0xF9, 0x04, 0xF9, 0x8E, 0xFE, 0x7A,
0xFF, 0xFF, 0xEF, 0xA6, 0xEF, 0xAA, 0xFD, 0x96, 0xEF, 0xB6, 0xEF, 0xAE, 0xEF, 0xBA, 0xFE, 0xFF
};
