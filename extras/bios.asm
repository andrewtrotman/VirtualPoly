;
;	BIOS.ASM
;	--------
;	BIOS for the iOS FLEX emulator

	org $F000
;
;	BOOT
;	----
;
BOOT
	;
	;	Initialise the stack and start
	;
BUFFER	EQU	$C100

	LDU #$A000
	LDS #$8000
	LBRA main

	;
	;	Include drivers for everything
	;
	INCLUDE serial.asm
	INCLUDE io.asm
	INCLUDE ata_ide.asm
    INCLUDE poly_compatibility.asm
	;
	;	Strings and constants
	;
	PRAGMA cescapes
bios_startup_message
	FCN "             Pinnated 6809\r\n   Copyright (C) 2021 Andrew Trotman\r\n"

;
;	MAIN
;	----
;
main
	;
	;	Initialise the serial port
	;
	LBSR io_init

	;
	;	Print the power-on message
	;
finish
	LEAX bios_startup_message,pcr
	LBSR io_puts

	;
	;	Initialise the IDE controller
	;
	LBSR FLEX_INIT

	;
	;	Boot FLEX
	;
	LDX #BUFFER
	LDA #$00
	LDB #$01
	LBSR FLEX_READ
	JMP BUFFER

	LBRA finish			; if we get here then start over (but it can't get here!)


;
;	6809 INTERRUPT VECTORS
;	----------------------
;
	org $FFF0

vector_reserved
	FDB BOOT
vector_swi3
	FDB BOOT
vector_swi2
	FDB BOOT
vector_firq
	FDB BOOT
vector_irq
	FDB BOOT
vector_swi
	FDB SWI_HANDLER
vector_nmi
	FDB BOOT
vector_reset
	FDB BOOT
