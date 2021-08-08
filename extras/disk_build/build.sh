new flex.dsk t254,s254
mount 0 flex.dsk
label 0 Flex
rdbootfile ../bootsector.bin
wrboot 0

mount 1 original_flex09_AB178.dsk
copy 1 0
umount 1

mount 1 flex_srces.dsk
copy 1 0
umount 1

mount 1 DIAG6809.DSK
copy 1 0
umount 1

mount 1 F77.DSK	
copy 1 0

umount 1
mount 1 XBASIC_2.DSK
copy 1 0
umount 1

mount 1 RBASIC.DSK
copy 1 0
umount 1

#mount 1 introl_c/IC1.DSK
#copy 1 0
#umount 1
#
#mount 1 introl_c/IC2.DSK
#copy 1 0
#umount 1
#
#mount 1 introl_c/IC3.DSK
#copy 1 0
#umount 1

mount 1 mccosh_C/SJT022.DSK
copy 1 0
umount 1

mount 1 advgames.dsk
copy 1 0
umount 1

mount 1 ADVENT.dsk
copy 1 0
umount 1

mount 1 ved.dsk
copy 1 0
umount 1

import 0 PATCH.TXT xxx=compress
link 0.FLEX.COR
umount 0
quit
