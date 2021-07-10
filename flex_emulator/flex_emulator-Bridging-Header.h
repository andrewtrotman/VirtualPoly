/*
	FLEX_EMULATOR-BRIDGING-HEADER.H
	-------------------------------
	Copyright (c) 2021 Andrew Trotman
*/

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
	{
#endif

	const void *machine_construct();
	void machine_destruct(const void *object);
	void machine_step(const void *object, uint64_t times);
	void machine_reset(const void *object);
	void machine_queue_key_press(const void *object, char key);
	int machine_dequeue_serial_output(const void *object);
	uint64_t machine_cycles_spent(const void *object);

	void machine_deserialise(const void *object);
	void machine_serialise(const void *object);

	uint8_t *get_saa5050_font(void);

#ifdef __cplusplus
	}
#endif

