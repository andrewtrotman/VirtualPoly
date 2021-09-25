;
;	RAM.ASM
;	-------
;	Methods to control the 6809 Dynamic Address Translation (DAT)

ram_dat_table_start EQU $E050
ram_dat_table_end	EQU $E060
ram_map1_select EQU $E060
ram_map2_select EQU $E070

;
;	RAM_INIT
;	--------
;	initialise the RAM DAT table and select memory map 1
;
ram_init
	BRA ram_init_after_data

ram_page_table_1
	FCB $00,$01,$02,$03,$04,$05,$06,$07,$08,$09,$0A,$0B,$0C,$0D,$0E,$0F

ram_init_after_data
	LDA #$FF
	STA ram_map1_select
	LEAX ram_page_table_1,pcr
	LDY #ram_dat_table_start
ram_init_more
	LDA ,X+
	COMA
	STA ,Y+
	CMPY #ram_dat_table_end
	BLO ram_init_more
ram_init_done

