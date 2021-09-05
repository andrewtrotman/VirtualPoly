cd ..
make
cd disk_build

../flex_vfs < build.input
cp flex.dsk /Users/andrew/programming/VirtualPoly/flex_emulator/flex.dsk
cp user.dsk /Users/andrew/programming/VirtualPoly/flex_emulator/user.dsk
