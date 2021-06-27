/*
	MISC.H
	------
	Based on original (c) 1994 R.P.Bellis
	Re-factoring (c) 2010 Andrew Trotman
	Ported to Mac 2021 Andrew Trotman
*/
#ifndef __misc_h__
#define __misc_h__

#include "typedefs.h"

/*
	BTST()
	------
*/
inline int btst(byte x, int n) { return (x & (1 << n)) ? 1 : 0; }
inline int btst(word x, int n) { return (x & (1 << n)) ? 1 : 0; }
inline int btst(dword x, int n) { return (x & (1L << n)) ? 1 : 0; }

/*
	BSET()
	------
*/
inline void bset(byte& x, int n) { x |= (1 << n); }
inline void bset(word& x, int n) { x |= (1 << n); }
inline void bset(dword& x, int n) { x |= (1L << n); }

/*
	BCLR()
	------
*/
inline void bclr(byte& x, int n) { x &= ~(1 << n); }
inline void bclr(word& x, int n) { x &= ~(1 << n); }
inline void bclr(dword& x, int n) { x &= ~(1L << n); }

/*
	EXTEND5()
	---------
*/
inline word extend5(byte x) { return (x & 0x10)? (word)x | 0xffe0 : (word)x; }

/*
	EXTEND8()
	---------
*/
inline word extend8(byte x) { return (x & 0x80) ? (word)x | 0xff00 : (word)x; }

#endif // __misc_h__

