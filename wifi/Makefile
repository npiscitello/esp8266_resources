CC = xtensa-lx106-elf-gcc
CFLAGS = -I. -mlongcalls
LDLIBS = -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc
LDFLAGS = -Teagle.app.v6.ld

wifi-0x00000.bin: wifi
	esptool.py elf2image $^

wifi: wifi.o

wifi.o: wifi.c

# flashing probs? try a lower baudrate, like 921600, 460800, 230400, or remove the arg alltogether
flash: wifi-0x00000.bin
	esptool.py -b 1500000 write_flash 0 wifi-0x00000.bin 0x10000 wifi-0x10000.bin

clean:
	rm -f wifi wifi.o wifi-0x00000.bin wifi-0x10000.bin
