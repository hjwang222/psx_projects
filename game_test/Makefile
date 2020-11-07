all:
	mkdir -p cd_root
	#-lm is for the math.h header and -lstdc++ is for c++ features like new/delete
	psx-gcc -O2 -Wno-narrowing -I../utils -std=c++11 -o game.elf main.cpp -lm -lfixmath -lstdc++ -lmeidogte
	elf2exe game.elf game.exe
	cp game.exe cd_root
	#cp data/*.tim cd_root
	#cp data/*.raw cd_root
	systemcnf game.exe > cd_root/system.cnf
	mkisofs -o game.hsf -V GAME -sysid PLAYSTATION cd_root
	mkpsxiso game.hsf game.bin /usr/local/psxsdk/share/licenses/infousa.dat
	rm -f game.hsf
	rm -rf cd_root
	cp game.bin /mnt/c/roms/game.bin
	cp game.cue /mnt/c/roms/game.cue

clean:
	rm -f game.bin game.cue game.exe game.elf