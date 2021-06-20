//
//  cpp_methods.hpp
//
//  Created by Andrew Trotman on 6/06/21.
//

#ifndef cpp_methods_hpp
#define cpp_methods_hpp

#include <stdio.h>
#include "computer.h"

extern "C"
	{
	const void *machine_construct()
		{
		computer *machine = new computer();
		machine->reset();
		return (const void *)machine;
		}

	void machine_destruct(const void *object)
		{
		delete (computer *)object;
		}

	void machine_step(const void *object)
		{
		((computer *)object)->step();
		}

	void machine_queue_key_press(const void *object, char key)
		{
		((computer *)object)->queue_key_press(key);
		}

	int machine_dequeue_serial_output(const void *object)
		{
		return ((computer *)object)->dequeue_serial_output();
		}

	long long machine_cycles_spent(const void *object)
		{
		return ((computer *)object)->cycles;
		}

	unsigned char *get_saa5050_font(void)
		{
//		return saa5050_font;
		return saa5055_font;
		}



}

#endif /* cpp_methods_hpp */
