//
//  cpp_methods.hpp
//
//  Created by Andrew Trotman on 6/06/21.
//

#ifndef cpp_methods_hpp
#define cpp_methods_hpp

#include <stdio.h>
#include "mc6809.h"

extern "C"
	{
	const void *machine_construct()
		{
		mc6809 *machine = new mc6809();
		machine->reset();
		return (const void *)machine;
		}

	void machine_destruct(const void *object)
		{
		delete (mc6809 *)object;
		}

	void machine_step(const void *object)
		{
		((mc6809 *)object)->step();
		}

	void machine_queue_key_press(const void *object, char key)
		{
		((mc6809 *)object)->queue_key_press(key);
		}

	int machine_dequeue_serial_output(const void *object)
		{
		return ((mc6809 *)object)->dequeue_serial_output();
		}
}

#endif /* cpp_methods_hpp */
