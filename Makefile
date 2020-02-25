
all: libcm3 appprj

libcm3:
	make -C libopencm3

appprj:
	make -C my-project
	cd my-project && arm-none-eabi-objcopy rau_mcu.elf -Oihex rau_mcu.hex

clean_libcm3:
	make -C libopencm3 clean

clean_appprj:
	make -C my-project clean

clean: clean_libcm3 clean_appprj
