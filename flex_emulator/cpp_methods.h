//
//  cpp_methods.hpp
//
//  Created by Andrew Trotman on 6/06/21.
//

#pragma once

#include <stdio.h>
#include "computer.h"

extern "C"
	{
	const void *machine_construct();
	void machine_destruct(const void *object);
	void machine_step(const void *object, uint64_t times);
	void machine_reset(const void *object);
	void machine_queue_key_press(const void *object, char key);
	int machine_dequeue_serial_output(const void *object);
	uint64_t machine_cycles_spent(const void *object);
	unsigned char *get_saa5050_font(void);
	void machine_deserialise(const void  *object);
	void machine_serialise(const void *object);
}
