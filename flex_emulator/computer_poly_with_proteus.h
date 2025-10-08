/*
	COMPUTER_POLY_WITH_PROTEUS.H
	----------------------------
	Copyright (c) 2025 Andrew Trotman
*/
#pragma once

#include "computer_poly_1.h"
#include "computer_proteus.h"

/*
	CLASS COMPUTER_POLY_WITH_PROTEUS
	--------------------------------
*/
class computer_poly_with_proteus : public computer_poly_1
	{
	friend std::ostream &operator<<(std::ostream &into, const computer_poly_with_proteus &simulator);
	friend std::istream &operator>>(std::istream &from, computer_poly_with_proteus &simulator);

	private:
		computer_proteus proteus;
		
	public:
		computer_poly_with_proteus();
		virtual ~computer_poly_with_proteus() {}

		virtual void step(uint64_t times);

		virtual void serialise(void) {}
		virtual void deserialise(void) {}

		virtual const char *change_disk(uint8_t drive, const char *filename);
	};

/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const computer_poly_with_proteus &simulator)
	{
	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, computer_poly_with_proteus &simulator)
	{
	return from;
	}
