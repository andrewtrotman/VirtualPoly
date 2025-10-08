/*
	CPP_METHODS.CPP
	---------------
	Copyright (c) 2021 Andrew Trotman
*/
#include "cpp_methods.h"
#include "computer_arrow.h"
#include "computer_poly_1.h"
#include "computer_proteus.h"
#include "computer_poly_with_proteus.h"

/*
	This is necessary in order for the Z80 emulator to be able to access the 6809 address space.
*/
computer_proteus *proteus_server;


/*
	MACHINE_CONSTRUCT()
	-------------------
*/
const void *machine_construct(machine_type type)
	{
	computer *machine;

	if (type == PINNATED)
		machine = new computer();
	else if (type == ARROW)
		machine = new computer_arrow();
	else if (type == POLY_1)
		machine = new computer_poly_1();
	else if (type == PROTEUS)
		machine = new computer_proteus();
	else
		machine = new computer_poly_with_proteus();

	machine->reset();
	return (const void *)machine;
	}

/*
	MACHINE_DESTRUCT()
	------------------
*/
void machine_destruct(const void *object)
	{
	delete (computer *)object;
	}

/*
	MACHINE_DESERIALISE()
	---------------------
*/
void machine_deserialise(const void  *object)
	{
	((computer *)object)->deserialise();
	}

/*
	MACHINE_SERIALISE()
	-------------------
*/
void machine_serialise(const void *object)
	{
	((computer *)object)->serialise();
	}

/*
	MACHINE_STEP()
	--------------
*/
void machine_step(const void *object, uint64_t times)
	{
	((computer *)object)->step(times);
	}

/*
	MACHINE_RESET()
	---------------
*/
void machine_reset(const void *object)
	{
	((computer *)object)->reset();
	}

/*
	MACHINE_CYCLES_SPENT()
	----------------------
*/
uint64_t machine_cycles_spent(const void *object)
	{
	return ((computer *)object)->cycles;
	}

/*
	MACHINE_REWIND_CYCLES_SPENT()
	-----------------------------
*/
void machine_rewind_cycles_spent(const void *object)
	{
	((computer *)object)->cycles = 0;
	}

/*
	MACHINE_SET_CYCLES_SPENT()
	--------------------------
*/
void machine_set_cycles_spent(const void *object, uint64_t new_value)
    {
    ((computer *)object)->cycles = new_value;
    }


/*
	MACHINE_QUEUE_KEY_PRESS()
	-------------------------
*/
void machine_queue_key_press(const void *object, char key)
	{
	((computer *)object)->queue_key_press(key);
	}

/*
	MACHINE_QUEUE_KEY_RELEASE()
	---------------------------
*/
void machine_queue_key_release(const void *object, char key)
	{
	((computer *)object)->queue_key_release(key);
	}


/*
	MACHINE_DEQUEUE_SERIAL_OUTPUT()
	-------------------------------
*/
int machine_dequeue_serial_output(const void *object)
	{
	return ((computer *)object)->dequeue_serial_output();
	}

/*
	MACHINE_CHANGE_DISK()
	---------------------
	Returns the name of the FLEX disk
*/
const char *machine_change_disk(const void *object, uint8_t drive, const char *filename)
	{
	return ((computer *)object)->change_disk(drive, filename);
	}

/*
	MACHINE_GET_SCREEN_BUFFER()
	---------------------------
*/
const uint8_t *machine_get_screen_buffer(const void *object)
	{
	return ((computer *)object)->screen_buffer();
	}

/*
	MACHINE_DID_SCREEN_CHANGE()
	---------------------------
*/
bool machine_did_screen_change(const void *object)
	{
	return ((computer *)object)->did_screen_change();
	}

/*
	MACHINE_POLY_1_RENDER_SCREEN()
	------------------------------
*/
void machine_poly_1_render_screen(const void *object, void *screen_buffer, bool flash_state)
	{
	((computer *)object)->render((uint32_t *)screen_buffer, flash_state);
	}

/*
	GET_SAA5050_FONT()
	------------------
*/
uint8_t *get_saa5050_font(void)
	{
	return saa5050_font;
//	return saa5055_font;
	}

/*
	Z80_MEMORY_READ()
	-----------------
*/
byte z80_memory_read(word addr)
{
return proteus_server->memory[addr];			// doesn't call read() to avoid ROM and device-space decoding
}

/*
	Z80_MEMORY_WRITE()
	------------------
*/
void z80_memory_write(word addr, byte val)
{
proteus_server->memory[addr] = val;			// doesn't call write() to avoid ROM and device-space decoding
}

/*
	Z80_IO_READ()
	-------------
*/
byte z80_io_read(word addr)
{
return proteus_server->read(0xE000 + (addr & 0xFF));
}

/*
	Z80_IO_WRITE()
	--------------
*/
void z80_io_write(word addr, byte val)
{
return proteus_server->write(0xE000 + (addr & 0xFF), val);
}
