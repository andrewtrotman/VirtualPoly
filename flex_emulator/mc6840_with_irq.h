/*
	MC6840_WITH_IRQ.H
	-----------------
*/
#pragma once

#include "mc6840.h"
#include "computer_poly_1.h"

/*
	class MC6840_WITH_IRQ
	---------------------
*/
class mc6840_with_irq : public mc6840
{
private:
	computer_poly_1 *cpu;

public:
	mc6840_with_irq(computer_poly_1 *cpu) { this->cpu = cpu; }
	virtual ~mc6840_with_irq() {}

	virtual void irq(void)  { cpu->timer_irq(); }
	virtual void d_irq(void) { cpu->timer_d_irq(); }
} ;
