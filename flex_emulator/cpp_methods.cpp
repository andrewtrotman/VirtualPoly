/*
	CPP_METHODS.CPP
	---------------
	Copyright (c) 2021 Andrew Trotman
*/
#include "cpp_methods.h"
#include "computer_arrow.h"

/*
	MACHINE_CONSTRUCT()
	-------------------
*/
const void *machine_construct()
	{
	computer *machine = new computer_arrow();
//	computer *machine = new computer();
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

const uint8_t *machine_get_screen_buffer(const void *object)
	{
	return ((computer *)object)->screen_buffer();
	}

/*
	GET_SAA5050_FONT()
	------------------
*/
uint8_t *get_saa5050_font(void)
	{
//		return saa5050_font;
	return saa5055_font;
	}
