#
# Build file for the FLEX ROM
#
all: ROM_FLEX.cpp

ROM_FLEX.cpp : bios.asm io.asm serial.asm ata_ide.asm
	lwasm --format=srec --map=bios.map --list=bios.lst -o bios.F000 bios.asm
	srec_cat bios.F000 -offset -0xE000 -o bios.s19
	srec_cat bios.F000 -o ROM_FLEX.cpp -c_array

clean:
	rm *.s19 *.lst *.map *.o *.F000 ROM_FLEX.cpp
