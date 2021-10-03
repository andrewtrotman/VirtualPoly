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
	LDS #$9000
	LDA #$00
	TFR A,DP
	LBRA main

	;
	;	Include drivers for everything
	;
	INCLUDE serial.asm
	INCLUDE io.asm
	INCLUDE ata_ide.asm
    INCLUDE poly_compatibility.asm
    INCLUDE screen.asm
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
	;	Configure the RAM DAT page table
	;
	INCLUDE ram.asm

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
	;	Copy the SWI gateway methods into the FLEX address space
	;
	LDX	#BYTES_TO_COPY_TO_D370
	LDY	#$D370
FLEX_GATEWAY_COPY
	LDA	,X+
	STA	,Y+
	CMPX #BYTES_TO_COPY_TO_D370_END
	BNE	FLEX_GATEWAY_COPY

	;
	;	Boot FLEX
	;
	LDX #BUFFER
	LDA #$00
	LDB #$01
	LBSR FLEX_READ
;
;	Exit the BIOS and enter FLEX
;
	LDX	#BUFFER		; entry address
	PSHS X			; put it on the stack
	LDA #%00000000	; %EFHINZVC			; in other words, we're about to return from a FIRQ
	PSHS A			; store the flags in the stack

	STA 0xE040		; leave PROT mode after the RTI
	RTI				; go to the entry address ($C100)

;	JMP BUFFER

	LBRA finish			; if we get here then start over (but it can't get here!)

;
;	INTERRUPT HANDLERS
;	==================
;

;
; 	FLEX_HANDLER
;	------------
;	The FLEX interface.  Sits on SWI2.
;	This allows us to leave Protected mode and to call back in for FLEX compatibility
;
FLEX_HANDLER
    ;
    ; When SWI is called the next byte is the function
    ; so we extract it and increment the return address
    ;
    PSHS B				; Save B
    LDU $0B,S   		; Return address
    LDB ,U+     		; SWI function
    ASLB        		; Turn the function number into a table offset
    STU $0B,S   		; The new return address (after the function numner)

    LDU #FLEX_FUNCTION_TABLE
    LEAU B,U
    PULS B				; Restore B
    JSR [,U]			; do the call

	TFR D,U				; put A and B into U
    EXG A,CC			; put CC back onto the stack by swapping with A, storing A, and swapping back
    STA ,S				; note that STA affects the Z flag, so we have to restore the flags before saving D
    STU	$01,S			; Save A and B

	STA 0xE040			; leave PROT mode after the RTI
	RTI

FLEX_FUNCTION_TABLE
    FDB     FLEX_INCHNE 	;   00 INPUT CHARACTER W/O ECHO
    FDB     FLEX_TMOFF  	;   01 TIMER OFF ROUTINE
    FDB     FLEX_TMON   	;   02 TIMER ON ROUTINE
    FDB     FLEX_TMINT  	;   03 TIMER INITIALIZATION
    FDB     FLEX_TINIT  	;   04 TERMINAL INITIALIZATION
    FDB     FLEX_STAT   	;   05 CHECK TERMINAL STATUS
    FDB     FLEX_OUTCH  	;   06 OUTPUT CHARACTER
    FDB     FLEX_INCH   	;   07 INPUT CHARACTER W/ ECHO

	FDB		FLEX_READ		;	08 Read a single sector
	FDB		FLEX_WRITE		;	09 Write a single sector
	FDB		FLEX_VERIFY		;	0A Verify last sector written
	FDB		FLEX_RESTORE	;	0B Restore head to track #0
	FDB		FLEX_DRIVE		;	0C Select the specified drive
	FDB		FLEX_CHKRDY		;	0D Check for drive ready
	FDB		FLEX_QUICK		;	0E Quick check for drive ready
	FDB		FLEX_INIT		;	0F Driver initialize (cold start)
	FDB		FLEX_WARM		;	10 Driver initialize (warm start)
	FDB		FLEX_SEEK		;	11 Seek to specified track


;
;	FLEX Console Drivers
;
CALL_FLEX_INCHNE	EQU $D370+BYTES_CALL_FLEX_INCHNE-BYTES_TO_COPY_TO_D370
CALL_FLEX_TMOFF		EQU $D370+BYTES_CALL_FLEX_TMOFF-BYTES_TO_COPY_TO_D370
CALL_FLEX_TMON		EQU $D370+BYTES_CALL_FLEX_TMON-BYTES_TO_COPY_TO_D370
CALL_FLEX_TMINT		EQU $D370+BYTES_CALL_FLEX_TMINT-BYTES_TO_COPY_TO_D370
CALL_FLEX_TINIT		EQU $D370+BYTES_CALL_FLEX_TINIT-BYTES_TO_COPY_TO_D370
CALL_FLEX_STAT		EQU $D370+BYTES_CALL_FLEX_STAT-BYTES_TO_COPY_TO_D370
CALL_FLEX_OUTCH		EQU $D370+BYTES_CALL_FLEX_OUTCH-BYTES_TO_COPY_TO_D370
CALL_FLEX_INCH		EQU $D370+BYTES_CALL_FLEX_INCH-BYTES_TO_COPY_TO_D370
;
;	FLEX disk drivers
;
CALL_FLEX_READ		EQU $D370+BYTES_CALL_FLEX_READ-BYTES_TO_COPY_TO_D370
CALL_FLEX_WRITE		EQU $D370+BYTES_CALL_FLEX_WRITE-BYTES_TO_COPY_TO_D370
CALL_FLEX_VERIFY	EQU $D370+BYTES_CALL_FLEX_VERIFY-BYTES_TO_COPY_TO_D370
CALL_FLEX_RESTORE	EQU $D370+BYTES_CALL_FLEX_RESTORE-BYTES_TO_COPY_TO_D370
CALL_FLEX_DRIVE		EQU $D370+BYTES_CALL_FLEX_DRIVE-BYTES_TO_COPY_TO_D370
CALL_FLEX_CHKRDY	EQU $D370+BYTES_CALL_FLEX_CHKRDY-BYTES_TO_COPY_TO_D370
CALL_FLEX_QUICK		EQU $D370+BYTES_CALL_FLEX_QUICK-BYTES_TO_COPY_TO_D370
CALL_FLEX_INIT		EQU $D370+BYTES_CALL_FLEX_INIT-BYTES_TO_COPY_TO_D370
CALL_FLEX_WARM		EQU $D370+BYTES_CALL_FLEX_WARM-BYTES_TO_COPY_TO_D370
CALL_FLEX_SEEK		EQU $D370+BYTES_CALL_FLEX_SEEK-BYTES_TO_COPY_TO_D370

BYTES_TO_COPY_TO_D370
BYTES_CALL_FLEX_INCHNE
	SWI
	FCB $00
	RTS
BYTES_CALL_FLEX_TMOFF
	SWI
	FCB $01
	RTS
BYTES_CALL_FLEX_TMON
	SWI
	FCB $02
	RTS
BYTES_CALL_FLEX_TMINT
	SWI
	FCB $03
	RTS
BYTES_CALL_FLEX_TINIT
	SWI
	FCB $04
	RTS
BYTES_CALL_FLEX_STAT
	SWI
	FCB $05
	RTS
BYTES_CALL_FLEX_OUTCH
	SWI
	FCB $06
	RTS
BYTES_CALL_FLEX_INCH
	SWI
	FCB $07
	RTS
BYTES_CALL_FLEX_READ
	SWI
	FCB $08
	RTS
BYTES_CALL_FLEX_WRITE
	SWI
	FCB $09
	RTS
BYTES_CALL_FLEX_VERIFY
	SWI
	FCB $0A
	RTS
BYTES_CALL_FLEX_RESTORE
	SWI
	FCB $0B
	RTS
BYTES_CALL_FLEX_DRIVE
	SWI
	FCB $0C
	RTS
BYTES_CALL_FLEX_CHKRDY
	SWI
	FCB $0D
	RTS
BYTES_CALL_FLEX_QUICK
	SWI
	FCB $0E
	RTS
BYTES_CALL_FLEX_INIT
	SWI
	FCB $0F
	RTS
BYTES_CALL_FLEX_WARM
	SWI
	FCB $10
	RTS
BYTES_CALL_FLEX_SEEK
	SWI
	FCB $11
	RTS
BYTES_TO_COPY_TO_D370_END


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
	FDB FLEX_HANDLER
vector_firq
	FDB BOOT
vector_irq
	FDB BOOT
vector_swi
;	FDB SWI_HANDLER
	FDB FLEX_HANDLER
vector_nmi
	FDB BOOT
vector_reset
	FDB BOOT
