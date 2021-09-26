;
;	SERIAL.ASM
;	----------
;	Control of MC68B50 serial port
;

serial_status 	EQU $E004
serial_control EQU $E004
serial_data		EQU $E005

;
; bit 0 is the receive data register full bit (1 = data ready to be read, 0 = no data)
; bit 1 is the transmit data register empty bit (1 = clear to send, 0 = busy)
;
serial_status_rdrf		EQU %00000001
serial_status_tdre		EQU %00000010

;
;	Using No start bits, 8 bit transmission and 1 stop bit
;	The 68B50 clock is 7,372,800
;	baud rate 115200  = E/64
;
serial_baud_rate_115200 EQU %00010110

;
;	When $03 is send to the control port the decice is reset
;
serial_reset EQU %00000011


;
;	FLEX_console_driver_vector_table
;	--------------------------------
;	This table is copied to $CD03 on startup and becomes the FLEX console driver vector table
;
FLEXWRM        EQU $CD03					;; FLEX warm start address
FLEX_console_driver_vector_table_address EQU $D3E5
FLEX_console_driver_vector_table
INCHNE  FDB CALL_FLEX_INCHNE	; INPUT CHARACTER W/O ECHO
IHNDLR  FDB FLEX_IHNDLR	; IRQ INTERRUPT HANDLER
SWIVEC  FDB vector_swi	; SWI3 VECTOR LOCATION
IRQVEC  FDB vector_irq	; IRQ VECTOR LOCATION
TMOFF   FDB CALL_FLEX_TMOFF	; TIMER OFF ROUTINE
TMON    FDB CALL_FLEX_TMON	; TIMER ON ROUTINE
TMINT   FDB CALL_FLEX_TMINT	; TIMER INITIALIZATION
MONITR  FDB FLEXWRM		; MONITOR ENTRY ADDRESS
TINIT   FDB CALL_FLEX_TINIT	; TERMINAL INITIALIZATION
STAT    FDB CALL_FLEX_STAT	; CHECK TERMINAL STATUS
OUTCH   FDB CALL_FLEX_OUTCH	; OUTPUT CHARACTER
INCH    FDB CALL_FLEX_INCH	; INPUT CHARACTER W/ ECHO
FLEX_console_driver_vector_table_end

;
;	FLEX_IHNDLR
;	-----------
;
FLEX_IHNDLR
	RTI

;
;	FLEX_TMINT
;	----------
;
FLEX_TMINT
	; Fall through

;
;	FLEX_TMON
;	---------
;
FLEX_TMON
	; Fall through

;
;	FLEX_TMOFF
;	----------
;
FLEX_TMOFF
	RTS

;
;	SERIAL_INIT
;	-----------
;
FLEX_TINIT		; TERMINAL INITIALIZATION
serial_init
	;
	;	Copy the FLEX driver table into RAM
	;
	LDX	#FLEX_console_driver_vector_table
	LDY	#FLEX_console_driver_vector_table_address
FLEX_TINIT_COPY
	LDA	,X+
	STA	,Y+
	CMPX	#FLEX_console_driver_vector_table_end
	BNE	FLEX_TINIT_COPY

	;
	;	Initialise the serial interface
	;
	LDA #serial_reset
	STA serial_control
	LDA #serial_baud_rate_115200
	STA serial_control
	RTS

;
;	SERIAL_PEEK
;	-----------
;	Check to see if a character is ready to be read, and if so then set
FLEX_STAT		; CHECK TERMINAL STATUS
serial_peek
	PSHS A
	LDA serial_status
	BITA #serial_status_rdrf
	PULS A
	RTS

;
;	SERIAL_GETCHAR
;	--------------
;	Blocking read of a character from the serial port, and return in A
;
FLEX_INCHNE		; INPUT CHARACTER W/O ECHO
serial_getchar
	LDA serial_status					; for a character to arrive
	BITA #serial_status_rdrf
	BEQ serial_getchar

	LDA serial_data					; read the character that has arrived
	RTS

;
;	SERIAL_GETCHAR_ECHO
;	-------------------
;	Blocking read of a character from the serial port, echo, and return in A
;
FLEX_INCH		; INPUT CHARACTER W/ ECHO
serial_getchar_echo
	BSR serial_getchar
	BSR serial_putchar
	RTS

;
;	SERIAL_PUTCHAR
;	--------------
;	Print the character in A
;
FLEX_OUTCH		; OUTPUT CHARACTER
serial_putchar
	PSHS A
	PSHS A

serial_putchar_wait
	LDA serial_status					; wait until clear to write this time around
	BITA #serial_status_tdre
	BEQ serial_putchar_wait
	PULS A

	STA serial_data					; write

serial_putchar_wait_2
	LDA serial_status					; wait until clear to write next time around
	BITA #serial_status_tdre
	BEQ serial_putchar_wait_2
	PULS A

	RTS
