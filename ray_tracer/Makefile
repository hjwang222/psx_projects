all:
	mkdir -p cd_root
	psx-gcc -O2 -Wno-narrowing -std=c++11 -I../utils -o ptx.elf main.cpp -lm -lfixmath
	elf2exe ptx.elf ptx.exe
	cp ptx.exe cd_root
	systemcnf ptx.exe > cd_root/system.cnf
	mkisofs -o ptx.hsf -V PTX -sysid PLAYSTATION cd_root
	mkpsxiso ptx.hsf ptx.bin /usr/local/psxsdk/share/licenses/infousa.dat
	rm -f ptx.hsf
	rm -rf cd_root
	cp ptx.bin /mnt/c/roms/ptx.bin
	cp ptx.cue /mnt/c/roms/ptx.cue

clean:
	rm -f ptx.bin ptx.cue ptx.exe ptx.elf
