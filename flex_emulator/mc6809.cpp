/*
	MC6809.CPP
	----------
	Based on original (c) 1994 R.P.Bellis
	various bug fixes by (Dec 2003) BDA
	Substantial re-factoring (c) 2010 Andrew Trotman
	Ported to Mac 2021 Andrew Trotman
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mc6809.h"

/*
	MC6809::MC6809()
	----------------
*/
mc6809::mc6809() :
	a(acc.byte.a),			// set up the pointers to the accumulators
	b(acc.byte.b),
	d(acc.d)
	{
	memset(memory, 0, sizeof(memory));
	cycles = 0;
	post_byte = 0;
	}

/*
	MC6809::RESET()
	---------------
*/
void mc6809::reset(void)
	{
	pc = read_word(0xfffe);
	dp = 0x00;			/* Direct page register = 0x00 */
	cc.all = 0x00;		/* Clear all flags */
	cc.bit.i = 1;		/* IRQ disabled */
	cc.bit.f = 1;		/* FIRQ disabled */
	firqpend = 0;		/* No FIRQ pending */
	irqpend = 0;		/* No IRQ pending */
	}

/*
	MC6809::EXECUTE()
	-----------------
*/
void mc6809::execute(void)
	{
	long long_branch_happened = 0;

	ir = fetch();

	/*
		get the addressing mode
	*/
	switch (ir & 0xf0)
		{
		case 0x00:
		case 0x90:
		case 0xd0:
			mode = direct;
			break;
		case 0x20:
			mode = relative;
			break;
		case 0x30:
		case 0x40:
		case 0x50:
			if (ir < 0x34)
				mode = indexed;
			else if (ir < 0x38)
				mode = immediate;
			else
				mode = inherent;
			break;
		case 0x60:
		case 0xa0:
		case 0xe0:
			mode = indexed;
			break;
		case 0x70:
		case 0xb0:
		case 0xf0:
			mode = extended;
			break;
		case 0x80:
		case 0xc0:
			if (ir == 0x8d)
				mode = relative;
			else
				mode = immediate;
			break;
		case 0x10:
			switch (ir & 0x0f)
				{
				case 0x02:
				case 0x03:
				case 0x09:
				case 0x0d:
				case 0x0e:
				case 0x0f:
					mode = inherent;
					break;
				case 0x06:
				case 0x07:
					mode = relative;
					break;
				case 0x0a:
				case 0x0c:
					mode = immediate;
					break;
				case 0x00:
				case 0x01:
					ir <<= 8;
					ir |= fetch();
					switch (ir & 0xf0)
						{
						case 0x20:
							mode = relative;
							break;
						case 0x30:
							mode = inherent;
							break;
						case 0x80:
						case 0xc0:
							mode = immediate;
							break;
						case 0x90:
						case 0xd0:
							mode = direct;
							break;
						case 0xa0:
						case 0xe0:
							mode = indexed;
							break;
						case 0xb0:
						case 0xf0:
							mode = extended;
							break;
						}
					break;
				}
			break;
		}

	/*
		Get the instruction
	*/
	switch (ir)
		{
		case 0x3a:
			abx();
			break;
		case 0x89:
		case 0x99:
		case 0xa9:
		case 0xb9:
			adca();
			break;
		case 0xc9:
		case 0xd9:
		case 0xe9:
		case 0xf9:
			adcb();
			break;
		case 0x8b:
		case 0x9b:
		case 0xab:
		case 0xbb:
			adda();
			break;
		case 0xcb:
		case 0xdb:
		case 0xeb:
		case 0xfb:
			addb();
			break;
		case 0xc3:
		case 0xd3:
		case 0xe3:
		case 0xf3:
			addd();
			break;
		case 0x84:
		case 0x94:
		case 0xa4:
		case 0xb4:
			anda();
			break;
		case 0xc4:
		case 0xd4:
		case 0xe4:
		case 0xf4:
			andb();
			break;
		case 0x1c:
			andcc();
			break;
		case 0x47:
			asra();
			break;
		case 0x57:
			asrb();
			break;
		case 0x07:
		case 0x67:
		case 0x77:
			asr();
			break;
		case 0x24:
			bcc();
			break;
		case 0x25:
			bcs();
			break;
		case 0x27:
			beq();
			break;
		case 0x2c:
			bge();
			break;
		case 0x2e:
			bgt();
			break;
		case 0x22:
			bhi();
			break;
		case 0x85:
		case 0x95:
		case 0xa5:
		case 0xb5:
			bita();
			break;
		case 0xc5:
		case 0xd5:
		case 0xe5:
		case 0xf5:
			bitb();
			break;
		case 0x2f:
			ble();
			break;
		case 0x23:
			bls();
			break;
		case 0x2d:
			blt();
			break;
		case 0x2b:
			bmi();
			break;
		case 0x26:
			bne();
			break;
		case 0x2a:
			bpl();
			break;
		case 0x20:
			bra();
			break;
		case 0x16:
			lbra();
			break;
		case 0x21:
			brn();
			break;
		case 0x8d:
			bsr();
			break;
		case 0x17:
			lbsr();
			break;
		case 0x28:
			bvc();
			break;
		case 0x29:
			bvs();
			break;
		case 0x4f: 		// BDA addded (undocumented)
		case 0x4e:
			clra();
			break;
		case 0x5f:  	// BDA addded (undocumented)
		case 0x5e:
			clrb();
			break;
		case 0x0f:
		case 0x6f:
		case 0x7f:
			clr();
			break;
		case 0x81:
		case 0x91:
		case 0xa1:
		case 0xb1:
			cmpa();
			break;
		case 0xc1:
		case 0xd1:
		case 0xe1:
		case 0xf1:
			cmpb();
			break;
		case 0x1083:
		case 0x1093:
		case 0x10a3:
		case 0x10b3:
			cmpd();
			break;
		case 0x118c:
		case 0x119c:
		case 0x11ac:
		case 0x11bc:
			cmps();
			break;
		case 0x8c:
		case 0x9c:
		case 0xac:
		case 0xbc:
			cmpx();
			break;
		case 0x1183:
		case 0x1193:
		case 0x11a3:
		case 0x11b3:
			cmpu();
			break;
		case 0x108c:
		case 0x109c:
		case 0x10ac:
		case 0x10bc:
			cmpy();
			break;
		case 0x43: 		// BDA addded (undocumented)
		case 0x42:
		case 0x1042:
			coma();
			break;
		case 0x53: 		// BDA addded (undocumented)
		case 0x52:
			comb();
			break;
		case 0x03: 		// BDA addded (undocumented)
		case 0x62:
		case 0x63: 		// BDA addded (undocumented)
		case 0x73: 		// BDA addded (undocumented)
			com();
			break;
		case 0x19:
			daa();
			break;
		case 0x4a: 		// BDA addded (undocumented)
		case 0x4b:
			deca();
			break;
		case 0x5a: 		// BDA addded (undocumented)
		case 0x5b:
			decb();
			break;
		case 0x0a: 		// BDA addded (undocumented)
		case 0x0b:
		case 0x6a: 		// BDA addded (undocumented)
		case 0x6b:
		case 0x7a: 		// BDA addded (undocumented)
		case 0x7b:
			dec();
			break;
		case 0x88:
		case 0x98:
		case 0xa8:
		case 0xb8:
			eora();
			break;
		case 0xc8:
		case 0xd8:
		case 0xe8:
		case 0xf8:
			eorb();
			break;
		case 0x1e:
			exg();
			break;
		case 0x4c:
			inca();
			break;
		case 0x5c:
			incb();
			break;
		case 0x0c:
		case 0x6c:
		case 0x7c:
			inc();
			break;
		case 0x0e:
		case 0x6e:
		case 0x7e:
			jmp();
			break;
		case 0x9d:
		case 0xad:
		case 0xbd:
			jsr();
			break;
		case 0x86:
		case 0x96:
		case 0xa6:
		case 0xb6:
			lda();
			break;
		case 0xc6:
		case 0xd6:
		case 0xe6:
		case 0xf6:
			ldb();
			break;
		case 0xcc:
		case 0xdc:
		case 0xec:
		case 0xfc:
			ldd();
			break;
		case 0x10ce:
		case 0x10de:
		case 0x10ee:
		case 0x10fe:
			lds();
			break;
		case 0xce:
		case 0xde:
		case 0xee:
		case 0xfe:
			ldu();
			break;
		case 0x8e:
		case 0x9e:
		case 0xae:
		case 0xbe:
			ldx();
			break;
		case 0x108e:
		case 0x109e:
		case 0x10ae:
		case 0x10be:
			ldy();
			break;
		case 0x32:
			leas();
			break;
		case 0x33:
			leau();
			break;
		case 0x30:
			leax();
			break;
		case 0x31:
			leay();
			break;
		case 0x48:
			lsla();
			break;
		case 0x58:
			lslb();
			break;
		case 0x08:
		case 0x68:
		case 0x78:
			lsl();
			break;
		case 0x44: 		// BDA addded (undocumented)
		case 0x45:
			lsra();
			break;
		case 0x54: 		// BDA addded (undocumented)
		case 0x55:
			lsrb();
			break;
		case 0x04: 		// BDA addded (undocumented)
		case 0x05:
		case 0x64: 		// BDA addded (undocumented)
		case 0x65:
		case 0x74: 		// BDA addded (undocumented)
		case 0x75:
			lsr();
			break;
		case 0x3d:
			mul();
			break;
		case 0x40: 		// BDA addded (undocumented)
		case 0x41:
			nega();
			break;
		case 0x50: 		// BDA addded (undocumented)
		case 0x51:
			negb();
			break;
		case 0x00: 		// BDA addded (undocumented)
		case 0x01:
		case 0x60: 		// BDA addded (undocumented)
		case 0x61:
		case 0x70: 		// BDA addded (undocumented)
		case 0x71:
			neg();
			break;
		case 0x02:		 		// BDA addded (undocumented NEG/COM combination instruction for DP)
			if (cc.bit.c)
				com();
			else
				neg();
			break;
		case 0x12:
			nop();
			break;
		case 0x8a:
		case 0x9a:
		case 0xaa:
		case 0xba:
			ora();
			break;
		case 0xca:
		case 0xda:
		case 0xea:
		case 0xfa:
			orb();
			break;
		case 0x1a:
			orcc();
			break;
		case 0x34:
			pshs();
			break;
		case 0x36:
			pshu();
			break;
		case 0x35:
			puls();
			break;
		case 0x37:
			pulu();
			break;
		case 0x3e: 		// BDA addded (undocumented)
			reset();
			break;
		case 0x49:
			rola();
			break;
		case 0x59:
			rolb();
			break;
		case 0x09:
		case 0x69:
		case 0x79:
			rol();
			break;
		case 0x46:
			rora();
			break;
		case 0x56:
			rorb();
			break;
		case 0x06:
		case 0x66:
		case 0x76:
			ror();
			break;
		case 0x3b:
			rti();
			break;
		case 0x39:
			rts();
			break;
		case 0x82:
		case 0x92:
		case 0xa2:
		case 0xb2:
			sbca();
			break;
		case 0xc2:
		case 0xd2:
		case 0xe2:
		case 0xf2:
			sbcb();
			break;
		case 0x1d:
			sex();
			break;
		case 0x97:
		case 0xa7:
		case 0xb7:
			sta();
			break;
		case 0xd7:
		case 0xe7:
		case 0xf7:
			stb();
			break;
		case 0xdd:
		case 0xed:
		case 0xfd:
			std();
			break;
		case 0x10df:
		case 0x10ef:
		case 0x10ff:
			sts();
			break;
		case 0xdf:
		case 0xef:
		case 0xff:
			stu();
			break;
		case 0x9f:
		case 0xaf:
		case 0xbf:
			stx();
			break;
		case 0x109f:
		case 0x10af:
		case 0x10bf:
			sty();
			break;
		case 0x80:
		case 0x90:
		case 0xa0:
		case 0xb0:
			suba();
			break;
		case 0xc0:
		case 0xd0:
		case 0xe0:
		case 0xf0:
			subb();
			break;
		case 0x83:
		case 0x93:
		case 0xa3:
		case 0xb3:
			subd();
			break;
		case 0x3f:
			swi();
			break;
		case 0x103f:
			swi2();
			break;
		case 0x113f:
			swi3();
			break;
		case 0x1f:
			tfr();
			break;
		case 0x4d:
			tsta();
			break;
		case 0x5d:
			tstb();
			break;
		case 0x0d:
		case 0x6d:
		case 0x7d:
			tst();
			break;
		case 0x1024:
			long_branch_happened = lbcc();
			break;
		case 0x1025:
			long_branch_happened = lbcs();
			break;
		case 0x1027:
			long_branch_happened = lbeq();
			break;
		case 0x102c:
			long_branch_happened = lbge();
			break;
		case 0x102e:
			long_branch_happened = lbgt();
			break;
		case 0x1022:
			long_branch_happened = lbhi();
			break;
		case 0x102f:
			long_branch_happened = lble();
			break;
		case 0x1023:
			long_branch_happened = lbls();
			break;
		case 0x102d:
			long_branch_happened = lblt();
			break;
		case 0x102b:
			long_branch_happened = lbmi();
			break;
		case 0x1026:
			long_branch_happened = lbne();
			break;
		case 0x102a:
			long_branch_happened = lbpl();
			break;
		case 0x1021:
			long_branch_happened = lbrn();
			break;
		case 0x1028:
			long_branch_happened = lbvc();
			break;
		case 0x1029:
			long_branch_happened = lbvs();
			break;
		default:	 		// BDA addded (make everything else a NOP to distinquish between 6309)
			nop();
	//		invalid("instruction");
			break;

		}
	cycles += cycle_cost(ir, post_byte);
	}

/*
	MC6809::REFREG()
	----------------
*/
word &mc6809::refreg(byte post)
	{
	post &= 0x60;
	post >>= 5;

	switch (post)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return u;
		default:
			return s;
		}
	}

/*
	MC6809::BYTEREFREG()
	--------------------
*/
byte &mc6809::byterefreg(int r)
	{
	if (r == 0x08)
		return a;
	else if (r == 0x09)
		return b;
	else if (r == 0x0a)
		return cc.all;
	else
		return dp;
	}

/*
	MC6809::WORDREFREG()
	--------------------
*/
word &mc6809::wordrefreg(int r)
	{
	switch (r)
		{
		case 0x00:
			return d;
		case 0x01:
			return x;
		case 0x02:
			return y;
		case 0x03:
			return u;
		case 0x04:
			return s;
		default:
			return pc;
		}
	}

/*
	MC6809::FETCH_OPERAND()
	-----------------------
*/
byte mc6809::fetch_operand(void)
	{
	byte ret = 0;
	word addr;

	if (mode == immediate)
		ret = fetch();
	else if (mode == relative)
		ret = fetch();
	else if (mode == extended)
		{
		addr = fetch_word();
		ret = read(addr);
		}
	else if (mode == direct)
		{
		addr = ((word)dp << 8) | fetch();
		ret = read(addr);
		}
	else if (mode == indexed)
		{
		post_byte = fetch();
		do_predecrement(post_byte);
		addr = do_effective_address(post_byte);
		ret = read(addr);
		do_postincrement(post_byte);
		}
	else
		invalid("addressing mode");

	return ret;
	}

/*
	MC6809::FETCH_WORD_OPERAND()
	----------------------------
*/
word mc6809::fetch_word_operand(void)
	{
	word addr, ret = 0;

	if (mode == immediate)
		ret = fetch_word();
	else if (mode == relative)
		ret = fetch_word();
	else if (mode == extended)
		{
		addr = fetch_word();
		ret = read_word(addr);
		}
	else if (mode == direct)
		{
		addr = (word)dp << 8 | fetch();
		ret = read_word(addr);
		}
	else if (mode == indexed)
		{
		post_byte = fetch();
		do_predecrement(post_byte);
		addr = do_effective_address(post_byte);
		do_postincrement(post_byte);
		ret = read_word(addr);
		}
	else
		invalid("addressing mode");

	return ret;
	}

/*
	MC6809::FETCH_EFFECTIVE_ADDRESS()
	---------------------------------
*/
word mc6809::fetch_effective_address(void)
	{
	word addr = 0;

	if (mode == extended)
		addr = fetch_word();
	else if (mode == direct)
		addr = (word)dp << 8 | fetch();
	else if (mode == indexed)
		{
		post_byte = fetch();
		do_predecrement(post_byte);
		addr = do_effective_address(post_byte);
		do_postincrement(post_byte);
		}
	else
		invalid("addressing mode");

	return addr;
	}

/*
	MC6809::DO_EFFECTIVE_ADDRESS()
	------------------------------
*/
word mc6809::do_effective_address(byte post)
	{
	word addr = 0;

	if ((post & 0x80) == 0x00)
		addr = refreg(post) + extend5(post & 0x1f);
	else
		{
		switch (post & 0x1f)
			{
			case 0x00:
			case 0x02:
				addr = refreg(post);
				break;
			case 0x01:
			case 0x03:
			case 0x11:
			case 0x13:
				addr = refreg(post);
				break;
			case 0x04:
			case 0x14:
				addr = refreg(post);
				break;
			case 0x05:
			case 0x15:
				addr = extend8(b) + refreg(post);		// fixed by BDA
				break;
			case 0x06:
			case 0x16:
				addr = extend8(a) + refreg(post);		// fixed by BDA
				break;
			case 0x08:
			case 0x18:
				addr = refreg(post) + extend8(fetch());
				break;
			case 0x09:
			case 0x19:
				addr = refreg(post) + fetch_word();
				break;
			case 0x0b:
			case 0x1b:
				addr = d + refreg(post);
				break;
			case 0x0c:
			case 0x1c:
				addr = extend8(fetch());		// fixed by BDA
				addr += pc;
				break;
			case 0x0d:
			case 0x1d:
				addr = fetch_word();		// fixed by BDA
				addr += pc;
				break;
			case 0x1f:
				addr = fetch_word();
				break;
			default:
				invalid("indirect addressing postbyte");
			break;
			}

		/*
			Do extra indirection
		*/
		if (post & 0x10)
			addr = read_word(addr);
		}

	return addr;
	}

/*
	MC6809::DO_POSTINCREMENT()
	--------------------------
*/
void mc6809::do_postincrement(byte post)
	{
	switch (post & 0x9f)
		{
		case 0x80:
			refreg(post) += 1;
			break;
		case 0x90:
			invalid("postincrement");
			break;
		case 0x81:
		case 0x91:
			refreg(post) += 2;
			break;
		}
	}

/*
	MC6809::DO_PREDECREMENT()
	-------------------------
*/
void mc6809::do_predecrement(byte post)
	{
	switch (post & 0x9f)
		{
		case 0x82:
			refreg(post) -= 1;
			break;
		case 0x92:
			invalid("predecrement");
			break;
		case 0x83: case 0x93:
			refreg(post) -= 2;
			break;
		}
	}


/*
	------------------------------------------------------------------------------------------------------------------------
	OPCODES, originally in mc6809in.c
	Updated and corrected by BDA and Soren Roug - December 2003
	Substantial re-factoring (c) 2010 Andrew Trotman
	------------------------------------------------------------------------------------------------------------------------
*/

/*
	MC6809::ABX()
	-------------
*/
void mc6809::abx(void)
	{
	x += b;
	}

/*
	MC6809::HELP_ADC()
	------------------
*/
void mc6809::help_adc(byte &x)
	{
	byte m, t;
	word w;

	m = fetch_operand();

	t = (x & 0x0f) + (m & 0x0f) + cc.bit.c;
	cc.bit.h = btst(t, 4);		// Half carry

	t = (x & 0x7f) + (m & 0x7f) + cc.bit.c;
	cc.bit.v = btst(t, 7);		// Bit 7 carry in

	w = x + m + cc.bit.c;
	cc.bit.c = btst(w, 8);		// Bit 7 carry out
	x = w & 0xff;

	cc.bit.v ^= cc.bit.c;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::ADCA()
	--------------
*/
void mc6809::adca(void)
	{
	help_adc(a);
	}

/*
	MC6809::ADCB()
	--------------
*/
void mc6809::adcb(void)
	{
	help_adc(b);
	}

/*
	MC6809::HELP_ADD()
	------------------
*/
void mc6809::help_add(byte &x)
	{
	byte m, t;
	word w;

	m = fetch_operand();

	t = (x & 0x0f) + (m & 0x0f);
	cc.bit.h = btst(t, 4);		// Half carry

	t = (x & 0x7f) + (m & 0x7f);
	cc.bit.v = btst(t, 7);		// Bit 7 carry in

	w = x + m;
	cc.bit.c = btst(w, 8);		// Bit 7 carry out
	x = w & 0xff;

	cc.bit.v ^= cc.bit.c;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::ADDA()
	--------------
*/
void mc6809::adda(void)
	{
	help_add(a);
	}

/*
	MC6809::ADDB()
	--------------
*/
void mc6809::addb(void)
	{
	help_add(b);
	}

void mc6809::addd(void)
	{
	word m, t;
	dword td;

	m = fetch_word_operand();

	t = (d & 0x7fff) + (m & 0x7fff);
	cc.bit.v = btst(t, 15);

	td = (dword)d + m;
	cc.bit.c = btst(td, 16);
	d = (word)(td & 0xffff);

	cc.bit.v ^= cc.bit.c;
	cc.bit.n = btst(d, 15);
	cc.bit.z = !d;
	}

/*
	MC6809::HELP_AND()
	------------------
*/
void mc6809::help_and(byte &x)
	{
	x = x & fetch_operand();
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	cc.bit.v = 0;
	}

/*
	MC6809::ANDA()
	--------------
*/
void mc6809::anda(void)
	{
	help_and(a);
	}

/*
	MC6809::ANDB()
	--------------
*/
void mc6809::andb(void)
	{
	help_and(b);		// fixed by BDA and ANDB
	}

/*
	MC6809::ANDCC()
	---------------
*/
void mc6809::andcc(void)
	{
	cc.all &= fetch();

	#ifdef NEVER
	/*
		BDA - add interrupt support
	*/
	if (!cc.bit.f && firqpend)	// FIRQ not masked
		do_firq();
	if (!cc.bit.i && irqpend)	// IRQ not masked
		do_irq();
	#endif
	}

/*
	MC6809::HELP_ASR()
	------------------
*/
void mc6809::help_asr(byte &x)
	{
	cc.bit.c = btst(x, 0);
	x >>= 1;

	if ((cc.bit.n = btst(x, 6)) != 0)
		bset(x, 7);

	cc.bit.z = !x;
	}

/*
	MC6809::ASRA()
	--------------
*/
void mc6809::asra(void)
	{
	help_asr(a);
	}

/*
	MC6809::ASRB()
	--------------
*/
void mc6809::asrb(void)
	{
	help_asr(b);
	}

/*
	MC6809::ASR()
	-------------
*/
void mc6809::asr(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);

	help_asr(m);
	write(addr, m);
	}

/*
	MC6809::BCC()
	-------------
*/
void mc6809::bcc(void)
	{
	do_br(!cc.bit.c);
	}

/*
	MC6809::LBCC()
	--------------
*/
long mc6809::lbcc(void)
	{
	return do_lbr(!cc.bit.c);
	}

/*
	MC6809::BCS()
	-------------
*/
void mc6809::bcs(void)
	{
	do_br(cc.bit.c);
	}

/*
	MC6809::LBCS()
	--------------
*/
long mc6809::lbcs(void)
	{
	return do_lbr(cc.bit.c);
	}

/*
	MC6809::BEQ()
	-------------
*/
void mc6809::beq(void)
	{
	do_br(cc.bit.z);
	}

/*
	MC6809::LBEQ()
	--------------
*/
long mc6809::lbeq(void)
	{
	return do_lbr(cc.bit.z);
	}

/*
	MC6809::BGE()
	-------------
*/
void mc6809::bge(void)
	{
	do_br(!(cc.bit.n ^ cc.bit.v));
	}

/*
	MC6809::LBGE()
	--------------
*/
long mc6809::lbge(void)
	{
	return do_lbr(!(cc.bit.n ^ cc.bit.v));
	}

/*
	MC6809::BGT()
	-------------
*/
void mc6809::bgt(void)
	{
	do_br(!(cc.bit.z | (cc.bit.n ^ cc.bit.v)));		// fixed by BDA
	}

/*
	MC6809::LBGT()
	--------------
*/
long mc6809::lbgt(void)
	{
	return do_lbr(!(cc.bit.z | (cc.bit.n ^ cc.bit.v)));
	}

/*
	MC6809::BHI()
	-------------
*/
void mc6809::bhi(void)
	{
	do_br(!(cc.bit.c | cc.bit.z));
	}

/*
	MC6809::LBHI()
	--------------
*/
long mc6809::lbhi(void)
	{
	return do_lbr(!(cc.bit.c | cc.bit.z));
	}

/*
	MC6809::BITA()
	--------------
*/
void mc6809::bita(void)
	{
	help_bit(a);
	}

/*
	MC6809::BITB()
	--------------
*/
void mc6809::bitb(void)
	{
	help_bit(b);
	}

/*
	MC6809::HELP_BIT()
	------------------
*/
void mc6809::help_bit(byte x)
	{
	byte t = x & fetch_operand();
	cc.bit.n = btst(t, 7);
	cc.bit.v = 0;
	cc.bit.z = !t;
	}

/*
	MC6809::BLE()
	-------------
*/
void mc6809::ble(void)
	{
	do_br(cc.bit.z | (cc.bit.n ^ cc.bit.v));
	}

/*
	MC6809::LBLE()
	--------------
*/
long mc6809::lble(void)
	{
	return do_lbr(cc.bit.z | (cc.bit.n ^ cc.bit.v));
	}

/*
	MC6809::BLS()
	-------------
*/
void mc6809::bls(void)
	{
	do_br(cc.bit.c | cc.bit.z);
	}

/*
	MC6809::LBLS()
	--------------
*/
long mc6809::lbls(void)
	{
	return do_lbr(cc.bit.c | cc.bit.z);
	}

/*
	MC6809::BLT()
	-------------
*/
void mc6809::blt(void)
	{
	do_br(cc.bit.n ^ cc.bit.v);
	}

/*
	MC6809::LBLT()
	--------------
*/
long mc6809::lblt(void)
	{
	return do_lbr(cc.bit.n ^ cc.bit.v);
	}

/*
	MC6809::BMI()
	-------------
*/
void mc6809::bmi(void)
	{
	do_br(cc.bit.n);
	}

/*
	MC6809::LBMI()
	--------------
*/
long mc6809::lbmi(void)
	{
	return do_lbr(cc.bit.n);
	}

/*
	MC6809::BNE()
	-------------
*/
void mc6809::bne(void)
	{
	do_br(!cc.bit.z);
	}

/*
	MC6809::LBNE()
	--------------
*/
long mc6809::lbne(void)
	{
	return do_lbr(!cc.bit.z);
	}

/*
	MC6809::BPL()
	-------------
*/
void mc6809::bpl(void)
	{
	do_br(!cc.bit.n);
	}

/*
	MC6809::LBPL()
	--------------
*/
long mc6809::lbpl(void)
	{
	return do_lbr(!cc.bit.n);
	}

/*
	MC6809::BRA()
	-------------
*/
void mc6809::bra(void)
	{
	do_br(1);
	}

/*
	MC6809::LBRA()
	--------------
*/
long mc6809::lbra(void)
	{
	return do_lbr(1);
	}

/*
	MC6809::BRN()
	-------------
*/
void mc6809::brn(void)
	{
	do_br(0);
	}

/*
	MC6809::LBRN()
	--------------
*/
long mc6809::lbrn(void)
	{
	return do_lbr(0);
	}

/*
	MC6809::BSR()
	-------------
*/
void mc6809::bsr(void)
	{
	byte x = fetch();
	write(--s, (byte)pc);
	write(--s, (byte)(pc >> 8));
	pc += extend8(x);
	}

/*
	MC6809::LBSR()
	--------------
*/
void mc6809::lbsr(void)
	{
	word x = fetch_word();
	write(--s, (byte)pc);
	write(--s, (byte)(pc >> 8));
	pc += x;
	}

/*
	MC6809::BVC()
	-------------
*/
void mc6809::bvc(void)
	{
	do_br(!cc.bit.v);
	}

/*
	MC6809::LBVC()
	--------------
*/
long mc6809::lbvc(void)
	{
	return do_lbr(!cc.bit.v);
	}

/*
	MC6809::BVS()
	-------------
*/
void mc6809::bvs(void)
	{
	do_br(cc.bit.v);
	}

/*
	MC6809::LBVS()
	--------------
*/
long mc6809::lbvs(void)
	{
	return do_lbr(cc.bit.v);
	}

/*
	MC6809::CLRA()
	--------------
*/
void mc6809::clra(void)
	{
	help_clr(a);
	}

/*
	MC6809::CLRB()
	--------------
*/
void mc6809::clrb(void)
	{
	help_clr(b);
	}

/*
	MC6809::CLR()
	-------------
*/
void mc6809::clr(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);
	help_clr(m);
	write(addr, m);
	}

/*
	MC6809::HELP_CLR()
	------------------
*/
void mc6809::help_clr(byte &x)
	{
	cc.all &= 0xf0;
	cc.all |= 0x04;
	x = 0;
	}

/*
	MC6809::CMPA()
	--------------
*/
void mc6809::cmpa(void)
	{
	help_cmp(a);
	}

/*
	MC6809::CMPB()
	--------------
*/
void mc6809::cmpb(void)
	{
	help_cmp(b);
	}

/*
	MC6809::HELP_CMP()
	------------------
	BDA - carry fix, with thanks to Soren Roug, Complete rewrite of function
*/
void mc6809::help_cmp(byte x)
	{
	byte m = fetch_operand();
	int t = x - m;

	cc.bit.v = btst((byte)(x^m^t^(t>>1)),7);
	cc.bit.c = btst((word)t,8);
	cc.bit.n = btst((byte)t, 7);
	cc.bit.z = !t;
	}

/*
	MC6809::CMPD()
	--------------
*/
void mc6809::cmpd(void)
	{
	help_cmp(d);
	}

/*
	MC6809::CMPX()
	--------------
*/
void mc6809::cmpx(void)
	{
	help_cmp(x);
	}

/*
	MC6809::CMPY()
	--------------
*/
void mc6809::cmpy(void)
	{
	help_cmp(y);
	}

/*
	MC6809::CMPU()
	--------------
*/
void mc6809::cmpu(void)
	{
	help_cmp(u);
	}

/*
	MC6809::CMPS()
	--------------
*/
void mc6809::cmps(void)
	{
	help_cmp(s);
	}

/*
	MC6809::HELP_CMP()
	------------------
	BDA - carry fix, with thanks to Soren Roug, Complete rewrite of function
*/
void mc6809::help_cmp(word x)
	{
	word m = fetch_word_operand();
	int t = x - m;

	cc.bit.v = btst((dword)(x^m^t^(t>>1)),15);
	cc.bit.c = btst((dword)t,16);
	cc.bit.n = btst((dword)t, 15);
	cc.bit.z = !t;
	}

/*
	MC6809::COMA()
	--------------
*/
void mc6809::coma(void)
	{
	help_com(a);
	}

/*
	MC6809::COMB()
	--------------
*/
void mc6809::comb(void)
	{
	help_com(b);
	}

/*
	MC6809::COM()
	-------------
*/
void mc6809::com(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);
	help_com(m);
	write(addr, m);
	}

/*
	MC6809::HELP_COM()
	------------------
*/
void mc6809::help_com(byte &x)
	{
	x = ~x;
	cc.bit.c = 1;
	cc.bit.v = 0;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::DAA()
	-------------
*/
void mc6809::daa(void)
	{
	word t;
	byte c = 0;
	byte lsn = (a & 0x0f);
	byte msn = (a & 0xf0) >> 4;

	if (cc.bit.h || (lsn > 9))
		c |= 0x06;

	if (cc.bit.c || (msn > 9) || ((msn > 8) && (lsn > 9)))
		c |= 0x60;

	t = (word)a + c;
	cc.bit.c = btst(t, 8);
	a = (byte)t;

	cc.bit.n = btst(a, 7);
	cc.bit.z = !a;
	}

/*
	MC6809::DECA()
	--------------
*/
void mc6809::deca(void)
	{
	help_dec(a);
	}

/*
	MC6809::DECB()
	--------------
*/
void mc6809::decb(void)
	{
	help_dec(b);
	}

/*
	MC6809::DEC()
	-------------
*/
void mc6809::dec(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);
	help_dec(m);
	write(addr, m);
	}

/*
	MC6809::HELP_DEC()
	------------------
*/
void mc6809::help_dec(byte &x)
	{
	cc.bit.v = (x == 0x80);
	x = x - 1;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::EORA()
	--------------
*/
void mc6809::eora(void)
	{
	help_eor(a);
	}

/*
	MC6809::EORB()
	--------------
*/
void mc6809::eorb(void)
	{
	help_eor(b);
	}

/*
	MC6809::HELP_EOR()
	------------------
*/
void mc6809::help_eor(byte &x)
	{
	x = x ^ fetch_operand();
	cc.bit.v = 0;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::DO_FIRQ()
	-----------------
	BDA - Add call to simulate FIRQ
*/
void mc6809::do_firq()
	{
	if (!cc.bit.f)	// FIRQ not masked
		{
		cc.bit.e = 0;	// not all regs on stack
		help_psh(0x81, s, u);	// save cc, pc only
		pc = read_word(0xfff6);	// fetch firq vector
		cc.bit.f = cc.bit.i = 1; // Mask while servicing
		firqpend = 0;	// Clear pending flag
		cycles += 10;
		}
	else
		firqpend = 1;	// FIRQ pending
	}

/*
	SWAP()
	------
*/
static void swap(byte &r1, byte &r2)
	{
	byte t;

	t = r1;
	r1 = r2;
	r2 = t;
	}

/*
	SWAP()
	------
*/
static void swap(word &r1, word &r2)
	{
	word t;

	t = r1;
	r1 = r2;
	r2 = t;
	}

/*
	MC6809::EXG()
	-------------
*/
void mc6809::exg(void)
	{
	int r1, r2;
	byte w = fetch();

	r1 = (w & 0xf0) >> 4;
	r2 = (w & 0x0f) >> 0;
	if (r1 <= 5)
		if (r2 > 5)
			invalid("exchange register");
		else
			swap(wordrefreg(r2), wordrefreg(r1));
	else if (r1 >= 8 && r2 <= 11)
		if (r2 < 8 || r2 > 11)
			invalid("exchange register");
		else
			swap(byterefreg(r2), byterefreg(r1));
	else
		invalid("exchange register");
	}

/*
	MC6809::INCA()
	--------------
*/
void mc6809::inca(void)
	{
	help_inc(a);
	}

/*
	MC6809::INCB()
	--------------
*/
void mc6809::incb(void)
	{
	help_inc(b);
	}

/*
	MC6809::INC()
	-------------
*/
void mc6809::inc(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);
	help_inc(m);
	write(addr, m);
	}

/*
	MC6809::HELP_INC()
	------------------
*/
void mc6809::help_inc(byte &x)
	{
	cc.bit.v = (x == 0x7f);
	x = x + 1;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::DO_IRQ()
	----------------
	BDA - Add call to simulate IRQ
*/
void mc6809::do_irq()
	{
	if (!cc.bit.i)	// IRQ not masked
		{
		cc.bit.e = 1;	// all regs on stack
		help_psh(0xff, s, u);
		pc = read_word(0xfff8);	// fetch irq vector
		cc.bit.i = 1;	// Mask IRQ while servicing
		irqpend = 0;	// Clear pending flag
		cycles += 19;
		}
	else
		irqpend = 1;		// IRQ pending
	}

/*
	MC6809::JMP()
	-------------
*/
void mc6809::jmp(void)
	{
	pc = fetch_effective_address();
	}

/*
	MC6809::JSR()
	-------------
*/
void mc6809::jsr(void)
	{
	word addr = fetch_effective_address();
	write(--s, (pc & 0xff));		//BDA removedc compiler warning
	write(--s, pc >> 8);
	pc = addr;
	}

/*
	MC6809::LDA()
	-------------
*/
void mc6809::lda(void)
	{
	help_ld(a);
	}

/*
	MC6809::LDB()
	-------------
*/
void mc6809::ldb(void)
	{
	help_ld(b);
	}

/*
	MC6809::HELP_LD()
	-----------------
*/
void mc6809::help_ld(byte &x)
	{
	x = fetch_operand();
	cc.bit.n = btst(x, 7);
	cc.bit.v = 0;
	cc.bit.z = !x;
	}

/*
	MC6809::LDD()
	-------------
*/
void mc6809::ldd(void)
	{
	help_ld(d);
	}

/*
	MC6809::LDX()
	-------------
*/
void mc6809::ldx(void)
	{
	help_ld(x);
	}

/*
	MC6809::LDY()
	-------------
*/
void mc6809::ldy(void)
	{
	help_ld(y);
	}

/*
	MC6809::LDS()
	-------------
*/
void mc6809::lds(void)
	{
	help_ld(s);
	}

/*
	MC6809::LDU()
	-------------
*/
void mc6809::ldu(void)
	{
	help_ld(u);
	}

/*
	MC6809::HELP_LD()
	-----------------
*/
void mc6809::help_ld(word &x)
	{
	x = fetch_word_operand();
	cc.bit.n = btst(x, 15);
	cc.bit.v = 0;
	cc.bit.z = !x;
	}

/*
	MC6809::LEAX()
	--------------
*/
void mc6809::leax(void)
	{
	x = fetch_effective_address();
	cc.bit.z = !x;
	}

/*
	MC6809::LEAY()
	--------------
*/
void mc6809::leay(void)
	{
	y = fetch_effective_address();
	cc.bit.z = !y;
	}

/*
	MC6809::LEAS()
	--------------
*/
void mc6809::leas(void)
	{
	s = fetch_effective_address();
	}

/*
	MC6809::LEAU()
	--------------
*/
void mc6809::leau(void)
	{
	u = fetch_effective_address();
	}

/*
	MC6809::LSLA()
	--------------
*/
void mc6809::lsla(void)
	{
	help_lsl(a);
	}

/*
	MC6809::LSLB()
	--------------
*/
void mc6809::lslb(void)
	{
	help_lsl(b);
	}

/*
	MC6809::LSL()
	-------------
*/
void mc6809::lsl(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);
	help_lsl(m);
	write(addr, m);
	}

/*
	MC6809::HELP_LSL()
	------------------
*/
void mc6809::help_lsl(byte &x)
	{
	cc.bit.c = btst(x, 7);
	cc.bit.v = btst(x, 7) ^ btst(x, 6);
	x <<= 1;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::LSRA()
	--------------
*/
void mc6809::lsra(void)
	{
	help_lsr(a);
	}

/*
	MC6809::LSRB()
	--------------
*/
void mc6809::lsrb(void)
	{
	help_lsr(b);
	}

/*
	MC6809::LSR()
	-------------
*/
void mc6809::lsr(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);

	help_lsr(m);
	write(addr, m);
	}

/*
	MC6809::HELP_LSR()
	------------------
*/
void mc6809::help_lsr(byte &x)
	{
	cc.bit.c = btst(x, 0);
	x >>= 1;	/* Shift word right */
	cc.bit.n = 0;
	cc.bit.z = !x;
	}

/*
	MC6809::MUL()
	-------------
*/
void mc6809::mul(void)
	{
	d = a * b;
	cc.bit.c = btst(b, 7);
	cc.bit.z = !d;
	}

/*
	MC6809::NEGA()
	--------------
*/
void mc6809::nega(void)
	{
	help_neg(a);
	}

/*
	MC6809::NEGB()
	--------------
*/
void mc6809::negb(void)
	{
	help_neg(b);
	}

/*
	MC6809::NEG()
	-------------
*/
void mc6809::neg(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);

	help_neg(m);
	write(addr, m);
	}

/*
	MC6809::HELP_NEG()
	------------------
	BDA - Determined rewrite of function, after some input by Soren Roug
*/
void mc6809::help_neg(byte &x)
	{
	int t = 0 - x;

	cc.bit.v = btst((byte)(x^t^(t>>1)),7);
	cc.bit.c = btst((word)t,8);
	cc.bit.n = btst((byte)t, 7);
	cc.bit.z = !t;
	x = t & 0xff;
	}

/*
	MC6809::DO_NMI()
	----------------
	BDA - Add call to simulate NMI
*/
void mc6809::do_nmi(void)
	{
	cc.bit.e = 1;	// all regs on stack
	help_psh(0xff, s, u);
	pc = read_word(0xfffc);	// fetch nmi vector
	cycles += 19;
	}

/*
	MC6809::NOP()
	-------------
*/
void mc6809::nop(void)
	{
	}

/*
	MC6809::ORA()
	-------------
*/
void mc6809::ora(void)
	{
	help_or(a);
	}

/*
	MC6809::ORB()
	-------------
*/
void mc6809::orb(void)
	{
	help_or(b);
	}

/*
	MC6809::HELP_OR()
	-----------------
*/
void mc6809::help_or(byte &x)
	{
	x = x | fetch_operand();
	cc.bit.v = 0;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::ORCC()
	--------------
*/
void mc6809::orcc(void)
	{
	cc.all |= fetch_operand();
	}

/*
	MC6809::PSHS()
	--------------
*/
void mc6809::pshs(void)
	{
	help_psh(fetch(), s, u);
	}

/*
	MC6809::PSHU()
	--------------
*/
void mc6809::pshu(void)
	{
	help_psh(fetch(), u, s);
	}

/*
	MC6809::HELP_PSH()
	------------------
*/
void mc6809::help_psh(byte w, word &s, word &u)
	{
	if (btst(w, 7))
		{
		write(--s, (byte)pc);
		write(--s, (byte)(pc >> 8));
		}
	if (btst(w, 6))
		{
		write(--s, (byte)u);
		write(--s, (byte)(u >> 8));
		}
	if (btst(w, 5))
		{
		write(--s, (byte)y);
		write(--s, (byte)(y >> 8));
		}
	if (btst(w, 4))
		{
		write(--s, (byte)x);
		write(--s, (byte)(x >> 8));
		}
	if (btst(w, 3))
		write(--s, (byte)dp);
	if (btst(w, 2))
		write(--s, (byte)b);
	if (btst(w, 1))
		write(--s, (byte)a);
	if (btst(w, 0))
		write(--s, (byte)cc.all);
	}

/*
	MC6809::PULS()
	--------------
*/
void mc6809::puls(void)
	{
	byte w = fetch();
	help_pul(w, s, u);
	}

/*
	MC6809::PULU()
	--------------
*/
void mc6809::pulu(void)
	{
	byte w = fetch();
	help_pul(w, u, s);
	}

/*
	MC6809::HELP_PUL()
	------------------
*/
void mc6809::help_pul(byte w, word &s, word &u)
	{
	if (btst(w, 0))
		cc.all = read(s++);
	if (btst(w, 1))
		a = read(s++);
	if (btst(w, 2))
		b = read(s++);
	if (btst(w, 3))
		dp = read(s++);
	if (btst(w, 4))
		{
		x = read_word(s);
		s += 2;
		}
	if (btst(w, 5))
		{
		y = read_word(s);
		s += 2;
		}
	if (btst(w, 6))
		{
		u = read_word(s);
		s += 2;
		}
	if (btst(w, 7))
		{
		pc = read_word(s);
		s += 2;
		}
	}

/*
	MC6809::ROLA()
	--------------
*/
void mc6809::rola(void)
	{
	help_rol(a);
	}

/*
	MC6809::ROLB()
	--------------
*/
void mc6809::rolb(void)
	{
	help_rol(b);
	}

/*
	MC6809::ROL()
	-------------
*/
void mc6809::rol(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);
	help_rol(m);
	write(addr, m);
	}

/*
	MC6809::HELP_ROL()
	------------------
*/
void mc6809::help_rol(byte &x)
	{
	int oc = cc.bit.c;
	cc.bit.v = btst(x, 7) ^ btst(x, 6);
	cc.bit.c = btst(x, 7);
	x = x << 1;
	if (oc)
		bset(x, 0);
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::RORA()
	--------------
*/
void mc6809::rora(void)
	{
	help_ror(a);
	}

/*
	MC6809::RORB()
	--------------
*/
void mc6809::rorb(void)
	{
	help_ror(b);
	}

/*
	MC6809::ROR()
	-------------
*/
void mc6809::ror(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);
	help_ror(m);
	write(addr, m);
	}

/*
	MC6809::HELP_ROR()
	------------------
*/
void mc6809::help_ror(byte &x)
	{
	int	oc = cc.bit.c;
	cc.bit.c = btst(x, 0);
	x = x >> 1;
	if (oc)
		bset(x, 7);
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::RTI()
	-------------
*/
void mc6809::rti(void)
	{
	help_pul(0x01, s, u);
	if (cc.bit.e)
		help_pul(0x7e, s, u); // BDA - fix stack pull for firq, help_pul(0xfe, s, u);
	help_pul(0x80, s, u); // BDA - fix stack pull for firq
	}

/*
	MC6809::RTS()
	-------------
*/
void mc6809::rts(void)
	{
	pc = read_word(s);
	s += 2;
	}

/*
	MC6809::SBCA()
	--------------
*/
void mc6809::sbca(void)
	{
	help_sbc(a);
	}

/*
	MC6809::SBCB()
	--------------
*/
void mc6809::sbcb(void)
	{
	help_sbc(b);
	}

/*
	MC6809::HELP_SBC()
	------------------
	BDA - carry fix, with thanks to Soren Roug, Complete rewrite of function
*/
void mc6809::help_sbc(byte &x)
	{
	byte m = fetch_operand();
	int t = x - m - cc.bit.c;

	cc.bit.v = btst((byte)(x^m^t^(t>>1)),7);
	cc.bit.c = btst((word)t,8);
	cc.bit.n = btst((byte)t, 7);
	cc.bit.z = !t;
	x = t & 0xff;
	}

/*
	MC6809::SEX()
	-------------
*/
void mc6809::sex(void)
	{
	cc.bit.n = btst(b, 7);
	cc.bit.z = !b;
	a = cc.bit.n ? 255 : 0;
	}

/*
	MC6809::STA()
	-------------
*/
void mc6809::sta(void)
	{
	help_st(a);
	}

/*
	MC6809::STB()
	-------------
*/
void mc6809::stb(void)
	{
	help_st(b);
	}

/*
	MC6809::HELP_ST()
	-----------------
*/
void mc6809::help_st(byte x)
	{
	word addr = fetch_effective_address();
	write(addr, x);
	cc.bit.v = 0;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::STD()
	-------------
*/
void mc6809::std(void)
	{
	help_st(d);
	}

/*
	MC6809::STX()
	-------------
*/
void mc6809::stx(void)
	{
	help_st(x);
	}

/*
	MC6809::STY()
	-------------
*/
void mc6809::sty(void)
	{
	help_st(y);
	}

/*
	MC6809::STS()
	-------------
*/
void mc6809::sts(void)
	{
	help_st(s);
	}

/*
	MC6809::STU()
	-------------
*/
void mc6809::stu(void)
	{
	help_st(u);
	}

/*
	MC6809::HELP_ST()
	-----------------
*/
void mc6809::help_st(word x)
	{
	word addr = fetch_effective_address();
	write_word(addr, x);
	cc.bit.v = 0;
	cc.bit.n = btst(x, 15);
	cc.bit.z = !x;
	}

/*
	MC6809::SUBA()
	--------------
*/
void mc6809::suba(void)
	{
	help_sub(a);
	}

/*
	MC6809::SUBB()
	--------------
*/
void mc6809::subb(void)
	{
	help_sub(b);
	}

/*
	MC6809::HELP_SUB()
	------------------
	BDA - carry fix, with thanks to Soren Roug, Complete rewrite of function
*/
void mc6809::help_sub(byte &x)
	{
	byte m = fetch_operand();
	int t = x - m;

	cc.bit.v = btst((byte)(x^m^t^(t>>1)),7);
	cc.bit.c = btst((word)t,8);
	cc.bit.n = btst((byte)t, 7);
	cc.bit.z = !t;
	x = t & 0xff;
	}

/*
	MC6809::SUBD()
	--------------
	BDA - carry fix, with thanks to Soren Roug, Complete rewrite of function
*/
void mc6809::subd(void)
	{
	word m = fetch_word_operand();
	int t = d - m;

	cc.bit.v = btst((dword)(d^m^t^(t>>1)),15);
	cc.bit.c = btst((dword)t,16);
	cc.bit.n = btst((dword)t, 15);
	cc.bit.z = !t;
	d = t &0xffff;
	}

/*
	MC6809::SWI()
	-------------
*/
void mc6809::swi(void)
	{
	cc.bit.e = 1;
	help_psh(0xff, s, u);
	cc.bit.f = cc.bit.i = 1;
	pc = read_word(0xfffa);
	}

/*
	MC6809::SWI2()
	--------------
*/
void mc6809::swi2(void)
	{
	cc.bit.e = 1;
	help_psh(0xff, s, u);
	pc = read_word(0xfff4);
	}

/*
	MC6809::SWI3()
	--------------
*/
void mc6809::swi3(void)
	{
	cc.bit.e = 1;
	help_psh(0xff, s, u);
	pc = read_word(0xfff2);
	}

/*
	MC6809::TFR()
	-------------
*/
void mc6809::tfr(void)
	{
	int r1, r2;
	byte w = fetch();

	r1 = (w & 0xf0) >> 4;
	r2 = (w & 0x0f) >> 0;
	if (r1 <= 5)
		if (r2 > 5)
			invalid("transfer register");
		else
			wordrefreg(r2) = wordrefreg(r1);
	else if (r1 >= 8 && r2 <= 11)
		if (r2 < 8 || r2 > 11)
			invalid("transfer register");
		else
			byterefreg(r2) = byterefreg(r1);
	else
		invalid("transfer register");
	}

/*
	MC6809::TSTA()
	--------------
*/
void mc6809::tsta(void)
	{
	help_tst(a);
	}

/*
	MC6809::TSTB()
	--------------
*/
void mc6809::tstb(void)
	{
	help_tst(b);
	}

/*
	MC6809::TST()
	-------------
*/
void mc6809::tst(void)
	{
	word addr = fetch_effective_address();
	byte m = read(addr);
	help_tst(m);
	}

/*
	MC6809::HELP_TST()
	------------------
*/
void mc6809::help_tst(byte x)
	{
	cc.bit.v = 0;
	cc.bit.n = btst(x, 7);
	cc.bit.z = !x;
	}

/*
	MC6809::DO_BR()
	---------------
*/
void mc6809::do_br(int test)
	{
	word offset = extend8(fetch_operand());
	if (test)
		pc += offset;
	}

/*
	MC6809::DO_LBR()
	----------------
*/
long mc6809::do_lbr(int test)
	{
	word offset = fetch_word_operand();
	if (test)
		{
		pc += offset;
		return 1;		// it was true
		}
	else
		return 0;		// it was false
	}


/*
	------------------------------------------------------------------------------------------------------------------------
	INSTRUCTION TIMINGS (c) 2010 Andrew Trotman
	------------------------------------------------------------------------------------------------------------------------
*/

static long page_0_instruction_times[] =
	{
	/*     0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F   */
	/*00*/ 6,  1,  1,  6,  6,  1,  6,  6,  6,  6,  6,  1,  6,  6,  3,  6,
	/*10*/ 0,  0,  2,  2,  1,  1,  5,  9,  1,  2,  3,  1,  3,  2,  8,  7,
	/*20*/ 3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
	/*30*/-4, -4, -4, -4,  5,  5,  5,  5,  1,  5,  3,  0, 21, 11,  1, 19,
	/*40*/ 2,  1,  1,  2,  2,  1,  2,  2,  2,  2,  2,  1,  2,  2,  1,  2,
	/*50*/ 2,  1,  1,  2,  2,  1,  2,  2,  2,  2,  2,  1,  2,  2,  1,  2,
	/*60*/-6,  1,  1, -6, -6,  1, -6, -6, -6, -6, -6,  1, -6, -6, -3, -6,
	/*70*/ 7,  1,  1,  7,  7,  1,  7,  7,  7,  7,  7,  1,  7,  7,  3,  7,
	/*80*/ 2,  2,  2,  4,  2,  2,  2,  1,  2,  2,  2,  2,  4,  7,  3,  1,
	/*90*/ 4,  4,  4,  6,  4,  4,  4,  4,  4,  4,  4,  4,  6,  7,  5,  5,
	/*A0*/-4, -4, -4, -6, -4, -4, -4, -4, -4, -4, -4, -4, -6, -7, -5, -5,
	/*B0*/ 5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  5,  7,  8,  6,  6,
	/*C0*/ 2,  2,  2,  4,  2,  2,  2,  1,  2,  2,  2,  2,  3,  1,  3,  1,
	/*D0*/ 4,  4,  4,  6,  4,  4,  4,  4,  4,  4,  4,  4,  5,  5,  5,  5,
	/*E0*/-4, -4, -4, -6, -4, -4, -4, -4, -4, -4, -4, -4, -5, -5, -5, -5,
	/*F0*/ 5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  5,  6,  6,  6,  6,
	};

static long page_1_instruction_times[] =
	{		// pre-byte = 0x10
	/*     0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F   */
	/*00*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*10*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*20*/ 0,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
	/*30*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 20,
	/*40*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*50*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*60*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*70*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*80*/ 0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  4,  0,
	/*90*/ 0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  6,  6,
	/*A0*/ 0,  0,  0, -7,  0,  0,  0,  0,  0,  0,  0,  0, -7,  0, -6, -6,
	/*B0*/ 0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  7,  7,
	/*C0*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  0,
	/*D0*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,
	/*E0*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -6, -6,
	/*F0*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
	};

static long page_2_instruction_times[] =
	{		// pre-byte = 0x11
	/*     0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F   */
	/*00*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*10*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*20*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*30*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 20,
	/*40*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*50*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*60*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*70*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*80*/ 0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,
	/*90*/ 0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,
	/*A0*/ 0,  0,  0, -7,  0,  0,  0,  0,  0,  0,  0,  0, -7,  0,  0,  0,
	/*B0*/ 0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,  0,
	/*C0*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*D0*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*E0*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/*F0*/ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	};

static long postbyte_bit_5_lo[] = {2, 3, 2, 3, 0, 1, 1, 0};

static long postbyte_bit_5_hi[] = {1, 4, 0, 4, 1, 5, 0, 5};

/*
	MC6809::CYCLE_COST()
	--------------------
*/
long mc6809::cycle_cost(word instruction, long long_branch_effect)
	{
	long cost = 0;

	if (instruction <= 0xFF)
		{
		cost = page_0_instruction_times[instruction];
		if (cost == 0)
			{
			if (instruction == 0x3B)			// RTI
				{
				if (cc.bit.e == 0)
					cost += 6;			// fast return (pull PC)
				else
					cost += 15;			// pull all registers
				}
			else if (instruction == 0x10)
				cost += 0;
			else // if (instruction == 0x11)
				cost += 0;
			}
		}
	else if (instruction <= 0x10FF)
		{
		cost = page_1_instruction_times[instruction & 0xFF];
		if (instruction >= 0x1020 && instruction <= 0x102F)
			if (long_branch_effect)
				cost++;			// a 6809 long branch takes 1 more opcode if true!!!
		}
	else // page 2
		cost = page_2_instruction_times[instruction & 0xFF];

	if (cost < 0)
		{
		cost = -cost;
		/*
			We have a post-byte to decode!
		*/
		if ((post_byte & 0x80) == 0)
			cost += 1;
		else
			{
			if (post_byte & 0x08)
				cost += postbyte_bit_5_hi[post_byte & 0x07];
			else
				cost += postbyte_bit_5_lo[post_byte & 0x07];

			if (post_byte & 0x10)		// indirect addressing cost
				cost += 3;
			}
		}

	return cost;
	}
