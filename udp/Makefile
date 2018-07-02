CC = xtensa-lx106-elf-gcc
CFLAGS = -I. -mlongcalls
LDLIBS = -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc
LDFLAGS = -Teagle.app.v6.ld

udp-0x00000.bin: udp
	esptool.py elf2image $^

udp: udp.o

udp.o: udp.c

# flashing probs? try a lower baudrate, like 921600, 460800, 230400, or remove the arg alltogether
flash: udp-0x00000.bin
	esptool.py -b 1500000 write_flash 0 udp-0x00000.bin 0x10000 udp-0x10000.bin

clean:
	rm -f udp udp.o udp-0x00000.bin udp-0x10000.bin
