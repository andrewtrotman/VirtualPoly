/*
	MC6850_WITH_MAC.H
	-----------------
*/
#ifndef MC6850_WITH_MAC_H_
#define MC6850_WITH_MAC_H_

#include "mc6850.h"

/*
	class MC6850_WITH_MAC
	---------------------
*/
class mc6850_with_mac : public mc6850
{
private:
	mc6890 *owner;

protected:
	virtual byte poll(void)
		{
		return kbhit();
		}
	virtual byte in(void)
		{
		return getch();
		}
	virtual void out(byte value)
		{
		putch(value );
		}

public:
	mc6850_with_mac(mc6809 *owner) :
		owner(owner),
		mc6850(),
		{
		create_window(title, mode);
		}

	~mc6850_with_mac() {}
} ;

#endif /* MC6850_WITH_MAC_H_ */
