/*
	MC6850.H
	--------
*/
#ifndef MC6850_H_
#define MC6850_H_

#include "typedefs.h"

/*
	class MC6850
	------------
	Motorola 6850 ACIA (Serial Port)
*/
class mc6850
{
private:
	byte baud_rate_register;

protected:
	virtual void reset(void);
	virtual byte poll(void);
	virtual byte in(void);
	virtual void out(byte value);

public:
	mc6850() { reset(); };
	virtual ~mc6850() {}

	byte read(word address);
	void write(word address, byte value);
} ;


#endif /* MC6850_H_ */
