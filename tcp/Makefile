CC = xtensa-lx106-elf-gcc
CFLAGS = -I. -mlongcalls
LDLIBS = -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc -ldriver
LDFLAGS = -Teagle.app.v6.ld

tcp-0x00000.bin: tcp
	esptool.py elf2image $^

tcp: tcp.o

tcp.o: tcp.c

# flashing probs? try a lower baudrate, like 921600, 460800, 230400, or remove the arg alltogether
flash: tcp-0x00000.bin
	esptool.py -b 1500000 write_flash 0 tcp-0x00000.bin 0x10000 tcp-0x10000.bin

clean:
	rm -f tcp tcp.o tcp-0x00000.bin tcp-0x10000.bin
