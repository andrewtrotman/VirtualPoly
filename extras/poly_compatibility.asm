;
;   POLY_COMPATIBILITY.ASM
;   ----------------------
;   Enough of a compatibility layer to make the Poly FLEX tools run (such as Omegasoft Pascal)
;

;
;   SWI_HANDLER
;   -----------
;
SWI_HANDLER
    ;
    ; When SWI is called the next byte is the function
    ; so we extract it and increment the return address
    ;
    LDX $0A,S   ; Return address
    LDB ,X+     ; SWI function
    ASLB        ; Turn the function number into a table offset
    STX $0A,S   ; The new return address (after the function numner)

    LDX #SWI_FUNCTION_TABLE
    JSR [B,X]
    RTI

SWI_FUNCTION_TABLE
    FDB     POLY_UNUSED  ;   00
    FDB     POLY_UNUSED  ;   01
    FDB     POLY_UNUSED  ;   02
    FDB     POLY_UNUSED  ;   03
    FDB     POLY_PAUSE   ;   04  PAUSE (if pause flag set)
    FDB     POLY_UNUSED  ;   05
    FDB     POLY_UNUSED  ;   06
    FDB     POLY_UNUSED  ;   07

;
;   POLY_UNUSED
;   -----------
;
POLY_UNUSED
    RTS
    
;
;   POLY_PAUSE
;   ----------
;   Do nothing - the PAUSE key is not pressed
;
POLY_PAUSE
    LDA #$FF
    STA $02,S           ;The location of B when SWI returns
    RTS



