;
;	ATA_IDE.ASM
;	-----------
;	Methods to fread and write via ATA/IDE interface (8-bit)
;

;
; Page 50 of the FLEX Adaptatopn Guide states:
;		Track numbers always begin with #0 and sector numbers always begin with #1.
; So there are at most 256 tracks each with 255 sectors on them!
;

;
;	Registers
;
ata_ide_data						EQU	$E010
ata_ide_feature					EQU	ata_ide_data+1
ata_ide_error						EQU	ata_ide_data+1
ata_ide_sector_count				EQU	ata_ide_data+2
ata_ide_sector_number			EQU	ata_ide_data+3
ata_ide_cylinder_low				EQU	ata_ide_data+4
ata_ide_cylinder_high			EQU	ata_ide_data+5
ata_ide_disk_head					EQU	ata_ide_data+6
ata_ide_status						EQU	ata_ide_data+7
ata_ide_command					EQU	ata_ide_data+7

ata_ide_command_read_sector	EQU	$20	; read a single sector
ata_ide_command_write_sector	EQU	$30	; write a single sector
ata_ide_command_identfy			EQU	$EC
ata_ide_command_set_feature	EQU	$EF	; set feature

ata_ide_feature_8_bit			EQU	$01	; 8-bit transfers

ata_ide_status_busy				EQU $80
ata_ide_status_drdy				EQU $40
ata_ide_status_drq				EQU $08
ata_ide_status_err				EQU $01

ata_ide_err_amnf					EQU $01
ata_ide_err_tk0nf					EQU $02
ata_ide_err_abrt					EQU $04
ata_ide_err_mcr					EQU $08
ata_ide_err_idnf					EQU $10
ata_ide_err_mc						EQU $20
ata_ide_err_unc					EQU $40
ata_ide_err_r						EQU $80

ata_ide_err_not_found			EQU $13

;
;	FLEX_DISK_DRIVER_ROUTINE_JUMP_TABLE
;	-----------------------------------
;	This table is copied to $DE00 on startup and becomes the FLEX disk driver jump table
;
FLEX_disk_driver_routine_jump_table_address EQU $DE00
FLEX_disk_driver_routine_jump_table
	JMP	CALL_FLEX_READ
	JMP	CALL_FLEX_WRITE
	JMP	CALL_FLEX_VERIFY
	JMP	CALL_FLEX_RESTORE
	JMP	CALL_FLEX_DRIVE
	JMP	CALL_FLEX_CHKRDY
	JMP	CALL_FLEX_QUICK
	JMP	CALL_FLEX_INIT
	JMP	CALL_FLEX_WARM
	JMP	CALL_FLEX_SEEK
FLEX_disk_driver_routine_jump_table_end

;
;	ATA_IDE_WAIT_FOR_NOT_BUSY
;	-------------------------
;	Spinlock until the BSY flag is clear
;
ata_ide_wait_for_not_busy
	PSHS	A
ata_ide_wait_for_not_busy_loop
	LDA	ata_ide_status
	BITA	#ata_ide_status_busy
	BNE	ata_ide_wait_for_not_busy_loop
	PULS	A
	RTS

;
;	ATA_IDE_WAIT_FOR_DRDY
;	---------------------
;	Spinlock until the DRDY flag is set
;
ata_ide_wait_for_drdy
	PSHS	A
ata_ide_wait_for_drdy_loop
	LDA	ata_ide_status
	BITA	#ata_ide_status_drdy
	BEQ	ata_ide_wait_for_drdy_loop
	PULS	A
	RTS

;
;	ATA_IDE_IDENTIFY
;	----------------
;	Load the device identify information into a 512-byte block pointed to by X
;
ata_ide_identify
	PSHS	A,X
	BSR	ata_ide_wait_for_not_busy

	LDA	#ata_ide_command_identfy
	STA	ata_ide_command
	BSR	ata_ide_wait_for_not_busy

	LBSR	FLEX_READ_256
	LBSR	FLEX_READ_256
	PULS	X,A
	RTS

;
;	ATA_IDE_ERR_TO_FLEX
;	-------------------
;	Destroys (A)
;
ata_ide_err_to_flex
	LDA	ata_ide_status
	EORA	#$50
	BNE	ata_ide_err_to_flex_error
	CLRB
	RTS

ata_ide_err_to_flex_error
	LDA	ata_ide_error
	BITA	#ata_ide_err_not_found
	BNE	ata_ide_err_to_flex_not_found

	BITA	#ata_ide_err_unc
	BNE	ata_ide_err_to_flex_unc

	BITA	#ata_ide_err_abrt
	BNE	ata_ide_err_to_flex_abrt

ata_ide_err_to_flex_unknown
	LDB	#$80
	BRA	ata_ide_err_to_flex_done
ata_ide_err_to_flex_abrt
	LDB	#$04
	BRA	ata_ide_err_to_flex_done
ata_ide_err_to_flex_unc
	LDB	#$08
	BRA	ata_ide_err_to_flex_done
ata_ide_err_to_flex_not_found
	LDB	#$10
ata_ide_err_to_flex_done
	RTS

;
;	FLEX_SECTOR_TO_LBA
;	------------------
;	FLEX track and sector numbers are one byte.
;	FLEX track numbers count from 00-FF.
;	FLEX sector numbers count from 01-FF.
;  FLEX sectors are 256 bytes.
;	So the largest FLEX volume is 16MB (well, 256*255*256 = 15.9375MB)
;	The smallest Compact Flash card I could find is 32MB.
;	So we'll use a disk geometory of 256 tracks of 255 sectors
;
;	On Entry:
;		(A) = Track Number
;		(B) = Sector Number
;	On Exit:
;		LBA registers loaded
;		(A) and (B) destroyed
;
FLEX_SECTOR_TO_LBA
	DECB				; because FLEX sectors count from 1, but ata/ide counts from 0
	PSHS	B			; save B
	CLRB				; push 16-bit sector number onto the stack
	PSHS	B
	LDB	#254		; convert track number into LBA
	MUL
	ADDD	,S			; add the sector number, resulting in LBA being in D (A high byte, B low byte)

	STB	ata_ide_sector_number	; write to the LBA registers
	STA	ata_ide_cylinder_low
	LDA	#$00
	STA	ata_ide_cylinder_high

	LDA	#$01							; read / write one sector
	STA	ata_ide_sector_count

	PULS	B
	PULS	B
	RTS

;
;	FLEX_READ_256
;	-------------
;	Read 256 bytes from the ATA/IDE controller into the memory pointed to by X
;
FLEX_READ_256
	CLRB
FLEX_READ_256_MORE
	LDA	ata_ide_data
	STA	,X+
	DECB
	BNE	FLEX_READ_256_MORE
	RTS

;
;	FLEX_READ
;	---------
;	Read a single sector
;	On Entry:
;		(X) = Address in memory where sector is to be placed.
;		(A) = Track Number
;		(B) = Sector Number
;	On Exit:
;		(X) May be destroyed
;		(A) May be destroyed
;		(B) = Error condition
;		(Z) = 1 if no error
;		    = 0 if an error
;
;	FLEX track numbers count from 00-FF.
;	FLEX sector numbers count from 01-FF.
;
FLEX_READ
	LBSR	ata_ide_wait_for_not_busy		; wait until not busy
	LBSR	ata_ide_wait_for_drdy

	LBSR	FLEX_SECTOR_TO_LBA				; write the track and sector numbers to the LBA regisgers

	LDA	#ata_ide_command_read_sector	; issue the command
	STA	ata_ide_command
	LBSR	ata_ide_wait_for_not_busy		; wait until not busy

	LBSR	FLEX_READ_256						; read the first 256 bytes
;	LDX	#$F000								; discard second half of IDE sector by writing it to ROM
;	LBSR	FLEX_READ_256						; read the second 256 bytes

	LBSR	ata_ide_err_to_flex
	RTS

;
;	FLEX_WRITE_256
;	--------------
;	Write 256 bytes to the ATA/IDE controller from the memory pointed to by X
;
FLEX_WRITE_256
	CLRB
FLEX_WRITE_256_MORE
	LDA	,X+
	STA	ata_ide_data
	DECB
	BNE	FLEX_WRITE_256_MORE
	RTS

;
;	FLEX_WRITE
;	----------
;
;	On Entry:
;		(X) = Address of 256 memory buffer containing data to be written to disk
;		(A) = Track Number
;		(B) = Sector Number
;	On Exit:
;		(X) May be destroyed
;		(A) May be destroyed
;		(B) = Error condition
;		(Z) = 1 if no error
;		    = 0 if an error
;
FLEX_WRITE
	LBSR	ata_ide_wait_for_not_busy		; wait until not busy
	LBSR	ata_ide_wait_for_drdy

	LBSR	FLEX_SECTOR_TO_LBA				; write the track and sector numbers to the LBA regisgers

	LDA	#ata_ide_command_write_sector	; issue the command
	STA	ata_ide_command
	LBSR	ata_ide_wait_for_not_busy		; wait until not busy

;	PSHS	X
	LBSR	FLEX_WRITE_256						; read the first 256 bytes
;	PULS	X
;	LBSR	FLEX_WRITE_256						; read the second 256 bytes

	LBSR	ata_ide_wait_for_not_busy		; wait until not busy
	LBSR	ata_ide_err_to_flex
	RTS

;
;	FLEX_DRIVE
;	----------
;	Select the specified drive
;	On Exit:
;		(B) = $0F if non-existent drive
;		    = Error condition otherwise
;		(Z) = 1 if no error
;		    = 0 if an error
;		(C) = 0 if no error
;		    = 1 if an error
;
FLEX_DRIVE
	LDA	$03,X						; load the drive number from the FCB
	BEQ	FLEX_DRIVE_ZERO
	CMPA	#$01
	BNE	FLEX_DRIVE_ERROR
	LDA	#$F0						; LBA mode, drive 1
	BRA	FLEX_DRIVE_CHECK
FLEX_DRIVE_ZERO
	LDA	#$E0						; LBA mode, drive 0
FLEX_DRIVE_CHECK
	STA	ata_ide_disk_head

FLEX_DRIVE_BUSY					; wait until not busy
	LDA	ata_ide_status
	BITA	#ata_ide_status_busy
	BNE	FLEX_DRIVE_BUSY

	EORA	#$50						; check for failure (i.e. no disk in drive)
	BNE	FLEX_DRIVE_ERROR

;	LBSR	ata_ide_wait_for_drdy

	CLRB								; set B=$00, Z=1, and C=0
	RTS
FLEX_DRIVE_ERROR
	LDB	#$1F						; load with fail state before we set the flags (next line)
	ASRB								; set B=$0F, Z=0, and C=1
	RTS

;
;	FLEX_INIT
;	---------
;	Driver initialize (cold start)
;
FLEX_INIT
	;
	;	Copy the FLEX disk driver jump table into RAM
	;
	LDX	#FLEX_disk_driver_routine_jump_table
	LDY	#FLEX_disk_driver_routine_jump_table_address
FLEX_INIT_COPY
	LDA	,X+
	STA	,Y+
	CMPX	#FLEX_disk_driver_routine_jump_table_end
	BNE	FLEX_INIT_COPY

	;
	;	Initialise the IDE interface
	;
	LDA	#$E0		; LBA3=0, MASTER, MODE=LBA
	STA	ata_ide_disk_head

	LBSR	ata_ide_wait_for_not_busy		; wait until not busy

	LDA	#$E0		; LBA3=0, MASTER, MODE=LBA
	STA	ata_ide_disk_head

	LDA	#ata_ide_feature_8_bit			; 8-bit I/O
	STA	ata_ide_feature

	LDA	#ata_ide_command_set_feature
	STA	ata_ide_command
	LBSR	ata_ide_wait_for_not_busy

	RTS

;
;	FLEX_WARM
;	---------
;	Driver initialize (warm start)
;
FLEX_WARM
	CLRB
	RTS

;
;	FLEX_VERIFY
;	-----------
;	Verify last sector written
;	On Exit:
;		(B) = Error condition
;		(Z) = 1 if no error
;		    = 0 if an error
;
FLEX_VERIFY
	CLRB
	RTS

;
;	FLEX_RESTORE
;	------------
;	Restore head to track #0
;	On Exit:
;		(B) = Error condition
;		(Z) = 1 if no error
;		    = 0 if an error
;
FLEX_RESTORE
	CLRB
	RTS

;
;	FLEX_SEEK
;	---------
;	Seek to specified track
;	On Exit:
;		(B) = Error condition
;		(Z) = 1 if no error
;		    = 0 if an error
;
FLEX_SEEK
	CLRB
	RTS

;
;	FLEX_QUICK
;	----------
;	Quick check for drive ready
;	On Exit:
;		(B) = Error condition
;		(Z) = 1 if drive ready
;		    = 0 if not ready
;		(C) = 0 if drive ready
;		    = 1 if not ready
;
FLEX_QUICK
	CLRB
	RTS

;
;	FLEX_CHKRDY
;	-----------
;	Check for drive ready
;	On Exit:
;		(B) = Error condition
;		(Z) = 1 if drive ready
;		    = 0 if not ready
;		(C) = 0 if drive ready
;		    = 1 if not ready
;
FLEX_CHKRDY
	CLRB
	RTS
	
