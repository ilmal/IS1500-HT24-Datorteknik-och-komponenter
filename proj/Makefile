SRC_DIR ?= ./
SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -or -name '*.S')
OBJECTS ?= $(notdir $(SOURCES))
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(OBJECTS:.S=.o)
LINKER ?= $(SRC_DIR)/dtekv-script.lds

TOOLCHAIN ?= riscv32-unknown-elf-
CFLAGS ?= -Wall -O3 -mabi=ilp32 -march=rv32imzicsr

# Exclude boot.o from OBJECTS to prevent multiple definitions
OBJECTS_NO_BOOT := $(filter-out boot.o,$(OBJECTS))

build: clean main.bin

main.elf: $(OBJECTS)
	$(TOOLCHAIN)gcc -nostartfiles -o $@ boot.o -T $(LINKER) $(OBJECTS_NO_BOOT) softfloat.a -lc -lgcc

%.o: $(SRC_DIR)/%.c
	$(TOOLCHAIN)gcc -c $(CFLAGS) -o $@ $<

%.o: $(SRC_DIR)/%.S
	$(TOOLCHAIN)gcc -c $(CFLAGS) -o $@ $<

main.bin: main.elf
	$(TOOLCHAIN)objcopy --output-target binary $< $@
	$(TOOLCHAIN)objdump -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt

TOOL_DIR ?= ./tools
run: main.bin
	make -C $(TOOL_DIR) "FILE_TO_RUN=$(CURDIR)/$<"
