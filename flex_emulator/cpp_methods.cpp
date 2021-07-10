/*
	CPP_METHODS.CPP
	---------------
*/
#include "cpp_methods.h"

/*
	MACHINE_CONSTRUCT()
	-------------------
*/
const void *machine_construct()
	{
	computer *machine = new computer();
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
	GET_SAA5050_FONT()
	------------------
*/
unsigned char *get_saa5050_font(void)
	{
//		return saa5050_font;
	return saa5055_font;
	}
