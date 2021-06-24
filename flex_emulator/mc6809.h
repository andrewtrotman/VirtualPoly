/*
	MC6809.H
	--------
	Based on original (c) 1993 R.P.Bellis
	Substantial re-factoring (c) 2010 Andrew Trotman
	Ported to Mac 2021 Andrew Trotman
*/
#pragma once

#include "usim.h"

/*
	class MC6809
	------------
*/
class mc6809 : public USim
	{
	/*
		Addressing Modes
	*/
	protected:
		enum { immediate = 0, relative = 0, inherent, extended, direct, indexed } mode;

	/*
		Registers
	*/
	public:
		word u, s;		// Stack Pointers
		word x, y;		// Index Registers
		byte dp;		// Direct Page Register
		union
			{
			word d;							// Combined accumulator D
			struct	{ byte b, a; } byte;	// Accumulators B and A
			} acc;
		byte &a;
		byte &b;
		word &d;
		union
			{
			byte all;	// Condition Code Registers
			struct
				{
				byte c:1;	// Carry
				byte v:1;	// Overflow
				byte z:1;	// Zero
				byte n:1;	// Negative
				byte i:1;	// IRQ disable
				byte h:1;	// Half carry
				byte f:1;	// FIRQ disable
				byte e:1;	// Entire
				} bit;
			} cc;
		word	firqpend, irqpend;		// Interrupt support was added by BDA
		qword cycles;						// number of clock cycles since power-on added by ASPT
		byte post_byte;

	protected:
		word &refreg(byte);
		byte &byterefreg(int);
		word &wordrefreg(int);

		byte fetch_operand(void);
		word fetch_word_operand(void);
		word fetch_effective_address(void);
		word do_effective_address(byte);
		void do_predecrement(byte);
		void do_postincrement(byte);

		void abx();
		void adca(), adcb();
		void adda(), addb(), addd();
		void anda(), andb(), andcc();
		void asra(), asrb(), asr();
		void bita(), bitb();
		void bcc();
		void bcs();
		void beq();
		void bge();
		void bgt();
		void bhi();
		void ble();
		void bls();
		void blt();
		void bmi();
		void bne();
		void bpl();
		void bra();
		void brn();
		void bsr();
		void bvc();
		void bvs();
		long lbcc();
		long lbcs();
		long lbeq();
		long lbge();
		long lbgt();
		long lbhi();
		long lble();
		long lbls();
		long lblt();
		long lbmi();
		long lbne();
		long lbpl();
		long lbra();
		long lbrn();
		void lbsr();
		long lbvc();
		long lbvs();
		void clra(), clrb(), clr();
		void cmpa(), cmpb();
		void cmpd(), cmpx(), cmpy(), cmpu(), cmps();
		void coma(), comb(), com();
		void cwai();
		void daa();
		void deca(), decb(), dec();
		void eora(), eorb();
		void exg();
		void inca(), incb(), inc();
		void jmp();
		void jsr();
		void lda(), ldb();
		void ldd(), ldx(), ldy(), lds(), ldu();
		void leax(), leay(), leas(), leau();
		void lsla(), lslb(), lsl();
		void lsra(), lsrb(), lsr();
		void mul();
		void nega(), negb(), neg();
		void nop();
		void ora(), orb(), orcc();
		void pshs(), pshu();
		void puls(), pulu();
		void rola(), rolb(), rol();
		void rora(), rorb(), ror();
		void rts();
		void sbca(), sbcb();
		void sex();
		void sta(), stb();
		void std(), stx(), sty(), sts(), stu();
		void suba(), subb();
		void subd();
		void sync();
		void tfr();
		void tsta(), tstb(), tst();

		void do_br(int);
		long do_lbr(int);

		void help_adc(byte&);
		void help_add(byte&);
		void help_and(byte&);
		void help_asr(byte&);
		void help_bit(byte);
		void help_clr(byte&);
		void help_cmp(byte);
		void help_cmp(word);
		void help_com(byte&);
		void help_dec(byte&);
		void help_eor(byte&);
		void help_inc(byte&);
		void help_ld(byte&);
		void help_ld(word&);
		void help_lsr(byte&);
		void help_lsl(byte&);
		void help_neg(byte&);
		void help_or(byte&);
		void help_psh(byte, word&, word&);
		void help_pul(byte, word&, word&);
		void help_ror(byte&);
		void help_rol(byte&);
		void help_sbc(byte&);
		void help_st(byte);
		void help_st(word);
		void help_sub(byte&);
		void help_sub(word&);
		void help_tst(byte);

	protected:
		virtual void execute(void);
		long cycle_cost(word opcode, long long_branch_effect);

	public:
		mc6809();
		virtual ~mc6809() {}

		virtual void reset(void);		// CPU reset
		virtual void do_nmi(void);
		virtual void do_firq(void);
		virtual void do_irq(void);
		virtual void swi(void);
		virtual void swi2(void);
		virtual void swi3(void);
		virtual void rti(void);
	} ;


/*
	OPERATOR<<()
	------------
*/
inline std::ostream &operator<<(std::ostream &into, const mc6809 &simulator)
	{
	operator<<(into, (USim &)simulator);

	into.write((char *)&simulator.u, sizeof(simulator.u));
	into.write((char *)&simulator.s, sizeof(simulator.s));
	into.write((char *)&simulator.x, sizeof(simulator.x));
	into.write((char *)&simulator.y, sizeof(simulator.y));
	into.write((char *)&simulator.dp, sizeof(simulator.dp));
	into.write((char *)&simulator.acc.d, sizeof(simulator.acc.d));
	into.write((char *)&simulator.cc.all, sizeof(simulator.cc.all));
	into.write((char *)&simulator.firqpend, sizeof(simulator.firqpend));
	into.write((char *)&simulator.irqpend, sizeof(simulator.irqpend));
	into.write((char *)&simulator.cycles, sizeof(simulator.cycles));
	into.write((char *)&simulator.post_byte, sizeof(simulator.post_byte));

	return into;
	}

/*
	OPERATOR>>()
	------------
*/
inline std::istream &operator>>(std::istream &from, mc6809 &simulator)
	{
	operator>>(from, (USim &)simulator);

	from.read((char *)&simulator.u, sizeof(simulator.u));
	from.read((char *)&simulator.s, sizeof(simulator.s));
	from.read((char *)&simulator.x, sizeof(simulator.x));
	from.read((char *)&simulator.y, sizeof(simulator.y));
	from.read((char *)&simulator.dp, sizeof(simulator.dp));
	from.read((char *)&simulator.acc.d, sizeof(simulator.acc.d));
	from.read((char *)&simulator.cc.all, sizeof(simulator.cc.all));
	from.read((char *)&simulator.firqpend, sizeof(simulator.firqpend));
	from.read((char *)&simulator.irqpend, sizeof(simulator.irqpend));
	from.read((char *)&simulator.cycles, sizeof(simulator.cycles));
	from.read((char *)&simulator.post_byte, sizeof(simulator.post_byte));

	return from;
	}

