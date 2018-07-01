CC = xtensa-lx106-elf-gcc
CFLAGS = -I. -mlongcalls
LDLIBS = -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc
LDFLAGS = -Teagle.app.v6.ld

webpage-0x00000.bin: webpage
	esptool.py elf2image $^

webpage: webpage.o

webpage.o: webpage.c

# flashing probs? try a lower baudrate, like 921600, 460800, 230400, or remove the arg alltogether
flash: webpage-0x00000.bin
	esptool.py -b 1500000 write_flash 0 webpage-0x00000.bin 0x10000 webpage-0x10000.bin

clean:
	rm -f webpage webpage.o webpage-0x00000.bin webpage-0x10000.bin
