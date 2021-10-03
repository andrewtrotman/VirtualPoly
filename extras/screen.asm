;
;	SCREEN.ASM
;	----------
;	Writing to the SAA5050 Teletext screen
;

	org			$E800
screen_base
	RMB 40*24
screen_end
screen_cursor_x_high
	RMB 1
screen_cursor_x
	RMB 1
screen_cursor_y_high
	RMB 1
screen_cursor_y
	RMB 1
	reorg

;
;	SCREEN_INIT
;	-----------
;
screen_init
	PSHS	X,D
	;
	;	Set up the cursor
	;
	LDD		#$0000
	STD		screen_cursor_x
	STD		screen_cursor_y
	;
	;	blank the screen
	;
	LDX		#screen_base
	LDD		#$0000
screen_init_blank_more
	STD		,X++
	CMPX	#screen_end
	BLT		screen_init_blank_more

	PULS	D,X
	RTS

;
;	SCREEN_PUTCHAR
;	--------------
;	Character to print is in A
;
screen_putchar
	;
	; check for special characters
	;
	CMPA	#$00
	BEQ		screen_putchar_done
	CMPA	#$0A
	BEQ		screen_key_line_feed
	CMPA	#$0D
	BEQ		screen_key_carrage_return
	;
	;		write to the screen
	;
	PSHS 	X,D
	PSHS	A
	BSR		screen_get_address
	TFR		D,X
	PULS	A
	STA 	screen_base,X
	;
	;		move cursor one to the right
	;
	BSR		screen_move_cursor_right

	PULS 	D,X
screen_putchar_done
	RTS

;
;	SCREEN_GET_ADDRESS
;	------------------
;	return (in D) the address of the cell represented by the cursor x and y locations
;
screen_get_address
	LDA		screen_cursor_y
	LDB		#40
	MUL
	ADDD	screen_cursor_x_high
	RTS

;
;	SCREEN_KEY_CARRAGE_RETURN
;	-------------------------
;
screen_key_carrage_return
	CLR		screen_cursor_x
	RTS

;
;	SCREEN_KEY_LINE_FEED
;	--------------------
;
screen_key_line_feed
	PSHS	A
	LDA		screen_cursor_y
	INCA
	STA		screen_cursor_y
	CMPA	#24
	BLT		screen_key_line_feed_done
	BSR		screen_scroll_up
	DEC		screen_cursor_y
screen_key_line_feed_done
	PULS	A
	RTS

;
;	SCREEN_MOVE_CURSOR_RIGHT
;	------------------------
;
screen_move_cursor_right
	PSHS	A
	LDA		screen_cursor_x
	CMPA	#39
	BLT		screen_move_cursor_right_not_at_eoln
	;
	;		move to start of next line
	;
	LDA		#$00
	STA		screen_cursor_x

	LDA		screen_cursor_y
	INCA
	STA		screen_cursor_y
	CMPA	#24
	BLT		screen_move_cursor_right_done
	;
	;		scroll then move to the start of the line
	;
	LDA		#23
	STA		screen_cursor_y
	BSR		screen_scroll_up
	BRA		screen_move_cursor_right_done
screen_move_cursor_right_not_at_eoln
	INCA
	STA		screen_cursor_x
screen_move_cursor_right_done
	PULS	A
	RTS

;
;	SCREEN_SCROLL_UP
;	----------------
;
screen_scroll_up
	PSHS 	X,Y,D
	;
	;	scroll the screen
	;
	LDX		#screen_base+40
	LDY		#screen_base
screen_scroll_up_copy_more
	LDD		,X++
	STD		,Y++
	CMPX	#screen_end
	BLT		screen_scroll_up_copy_more
	;
	;	blank the bottom line
	;
	LDD		#$0000
	LDX		#screen_end-40
screen_scroll_up_blank_more
	STD		,X++
	CMPX	#screen_end
	BLT		screen_scroll_up_blank_more

	PULS 	X,Y,D
	RTS
