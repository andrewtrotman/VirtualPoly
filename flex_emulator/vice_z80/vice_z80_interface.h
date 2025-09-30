/*
	VICE_Z80_INTERFACE.H
	--------------------
	Written (w) 2010 Andrew Trotman (andrew@cs.otago.ac.nz)

	This file contains the necessary parts to build the VICE z80
	emulator so that we can attach it to the Proteus Emulator
*/
#pragma once

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "z80regs.h"
#include "z80.h"

#define CLK_6809 1000000
#define CLK_Z80 (2 * CLK_6809)

typedef unsigned int UINT;
