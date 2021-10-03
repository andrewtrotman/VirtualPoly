;
;	IO.ASM
;	------
;
io_string_crlf
	FCB	$0A,00

;
;	IO_INIT
;	-------
;	Initialise the I/O console subsystem
;
FLEX_TINIT		; TERMINAL INITIALIZATION
io_init
	LBSR	serial_init
	LBSR	screen_init
	RTS

;
;	IO_PUTCHAR
;	----------
;	Print the character in A
;
FLEX_OUTCH		; OUTPUT CHARACTER
io_putchar
	LBSR	serial_putchar
	LBSR	screen_putchar
	RTS

;
;	IO_GETCHAR_ECHO
;	---------------
;
FLEX_INCH		; INPUT CHARACTER W/ ECHO
io_getchar_echo
	LBSR 	serial_getchar
	BSR 	io_putchar
	RTS

;
;	IO_PUTS
;	-------
;	Print the '\0' terminated string pointed to by X
;
io_puts
	PSHS	A,X
io_putchar_next
	LDA	,X+
	BEQ	io_puts_done
	LBSR	serial_putchar
	BRA	io_putchar_next
io_puts_done
	PULS	A,X
	RTS

;
;	IO_PUT_NYBBLE
;	-------------
;	Print the low order nybble of accumulator A
;
io_put_nybble
	PSHS	A
	ANDA	#$0F
	CMPA	#$09
	BLE 	io_put_nibble_1
	ADDA 	#$07		; for printing A..F
io_put_nibble_1
	ADDA 	#'0'
	LBSR 	io_putchar
	PULS 	A
	RTS

;
;	IO_PUT_BYTE
;	-----------
;	Print the value of accumualator A as as two nybble number
;
io_put_byte
	PSHS 	A
	LSRA
	LSRA
	LSRA
	LSRA
	BSR	io_put_nybble
	PULS 	A
	BSR	io_put_nybble
	RTS

;
;	IO_PUT_D
;	--------
;	Print the value of accumultor D as a four nybble number
;
io_put_d
	PSHS 	D
	BSR 	io_put_byte
	TFR 	B,A
	BSR 	io_put_byte
	PULS 	D
	RTS

;
;	IO_PUT_WORD
;	-----------
;	Print the value of register X as a four nybble number
;
io_put_word
	PSHS 	X,D
	TFR 	X,D
	BSR 	io_put_byte
	TFR 	B,A
	BSR 	io_put_byte
	PULS 	D,X
	RTS

;
;	IO_DUMP_MEMORY_16
;	-----------------
;	Dump 16 bytes pf memory pointed to by X
;
io_dump_memory_16
	PSHS	X,Y,D
	TFR	X,Y
	; print the address
	BSR	io_put_word
	LDA	#':'
	BSR	io_putchar

	; print the HEX
	LDB	#$10
io_dump_memory_16_more_bytes
	LDA 	,X+
	BSR 	io_put_byte
	LDA 	#' '
	BSR	io_putchar
	DECB
	BNE	io_dump_memory_16_more_bytes

	; print the ASCII
	LDA 	#' '
	BSR	io_putchar
	TFR	Y,X
	LDB	#$10
io_dump_memory_16_more_chars
	LDA 	,X+
	CMPA	#$20		; is ASCII ' ' or later
	BGE	io_dump_memory_16_printable
	LDA	#'.'
io_dump_memory_16_printable
	CMPA	#$80
	BLO	io_dump_memory_16_print
	LDA	#'.'
io_dump_memory_16_print
	LBSR 	io_putchar
	DECB
	BNE	io_dump_memory_16_more_chars

	; print the CRLF
	LDX #io_string_crlf
	BSR io_puts

	; Done
	PULS D,Y,X
	RTS

;
;	IO_DUMP_MEMORY
;	--------------
;	Dump memory pointed to by X for 16 lines of 16 bytes (256 bytes)
;
io_dump_memory
	PSHS	X,D
	LDA	#$10
io_dump_memory_more
	BSR	io_dump_memory_16
	LEAX	$10,X
	DECA
	BNE	io_dump_memory_more
	PULS	D,X
	RTS
